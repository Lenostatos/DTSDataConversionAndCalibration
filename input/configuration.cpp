
#include <iostream>

#include "configuration.hpp"
#include "tagging.hpp"
#include "../other/my_exceptions/tag_error.hpp"

namespace input {

configuration::configuration(const std::string & config_file, const tagging & tags)
{
  // Bouml preserved body begin 0001F98D

    // Extract the values of the single tags.
    for (const auto & value_tagging : tags.values)
        { values_.insert({value_tagging.first, extract_tag_value(value_tagging.second, config_file)}); }

    // Extract the tags and values of the sections
    for (const auto & section_tagging : tags.sections)
    {
        std::multimap < tag, tag_value > multi_section(extract_tags_and_values(section_tagging.second, config_file));
        std::map < tag, tag_value > section;

        for (const auto & multi_elem : multi_section)
        {
            if (multi_section.count(multi_elem.first) != 1)
            {
                std::cout << "Error: There are multiple \"" << multi_elem.first << "\" tags in the section at \"" << section_tagging.second.first << "\"!\n" << std::endl;
                throw my_exceptions::tag_error();
            }
            else
            {
                section.insert(multi_elem);
            }
        }

        sections_.insert({section_tagging.first, section});
    }

    // Extract the tags and values of the multi sections
    for (const auto & section_tagging : tags.multi_sections)
        { multi_sections_.insert({section_tagging.first, extract_tags_and_values(section_tagging.second, config_file)}); }

  // Bouml preserved body end 0001F98D
}

/**
 *  - Searches for the tag at the beginning of each line
 *  - (between the beginning of the line and the tag may only be whitespace characters)
 *  - (Whitespace characters are: single whitespace and tabulator characters.)
 *  - On success: checks the same line (until it's end or until a comment character (#)) for:
 *  - an equality sign and a tag value.
 *  - Tag, equality sign and tag value may only be separated by whitespace characters.
 *  - Leading and trailing whitespace characters around the tag value are omitted.
 *  - The tag value is returned.
 *
 *  \bug If the tag is assigned a value multiple times within the file, only the first occurrence is recognized.
 *
 *  \warning The tag shall not contain whitespace characters!
 *
 *  \param[in]  tag     A tag (without whitespace characters) to search for within the file.
 *  \param[in]  file    The file.
 *
 *  \exception my_exceptions::tag_error Thrown if the tag contains whitespace characters,
 *                                      could not be found or hasn't been assigned a value.
 *
 *  \return The tag's value.
 */
tag_value configuration::extract_tag_value(const std::string & tag, const std::string & config_file, char comment_character)
{
  // Bouml preserved body begin 0002158D
    // Checking the tag for whitespace characters
    // TODO: get these characters from the current locale somehow
    if (tag.find(" \t\v\f\r\n") != std::string::npos)
    {
        std::cerr << "\nError: Tag \"" << tag << "\" contains whitespace characters!\n" << std::endl;
        throw my_exceptions::tag_error();
    }

    std::cout << "Searching for tag \"" << tag << "\"...\n";

    size_t found_ind = config_file.find(tag);

    // if tag not found...
    if (found_ind == std::string::npos) // std::string::npos is returned by find() if nothing was found
    {
        std::cerr << "\nError: Tag \"" << tag << "\" not found!\n" << std::endl;
        throw my_exceptions::tag_error();
    }

    // TODO: get these characters from the current locale somehow
    const std::string inline_whitespaces = " \t";
    const std::string new_line_whitespaces = "\v\f\r\n";

    size_t first_non_ws_pos = 0;

    bool continue_search = false;

    // Searching for a tag-equality sign combination at the beginning of a not commented line
    do
    {
        continue_search = false;

        // if the tag is not at the beginning of the file...
        if (found_ind > 0)
        {
            // searching for last non-inline whitespace character before the tag
            size_t last_non_ws_pos = config_file.find_last_not_of(inline_whitespaces, found_ind - 1);

            // if a non-inline whitespace character is found but is not a newline whitespace, retry!
            if (last_non_ws_pos != std::string::npos && new_line_whitespaces.find(config_file[last_non_ws_pos]) == std::string::npos)
                { continue_search = true; }
        }

        // if everything is fine, continue...
        if (!continue_search)
        {
            // get position of first character after tag
            size_t after_tag_pos = found_ind + tag.length();

            // get position of next non-whitespace character after the tag
            first_non_ws_pos = config_file.find_first_not_of(inline_whitespaces, after_tag_pos);

            // if that non-whitespace character is NOT an equality sign, retry!
            if (config_file[first_non_ws_pos] != '=')
            {
                continue_search = true;
                std::cout << "Warning: Found and ignored uncommented tag \"" << tag << "\" without an equality sign!" << std::endl;
            }
        }

        if (continue_search) { found_ind = config_file.find(tag, found_ind + 1); }

    } while (continue_search && found_ind != std::string::npos);

    // if tag not found...
    if (found_ind == std::string::npos)
    {
        std::cerr << "\nError: Tag \"" << tag << "\" not found!\n" << std::endl;
        throw my_exceptions::tag_error();
    }

    size_t start_ind = first_non_ws_pos + 1; // get index of first character after the equality sign

    // get index of first newline whitespace or comment character (hash sign) (Windows may use "\r\n" line endings!)
    size_t end_ind = config_file.find_first_of(comment_character + new_line_whitespaces, start_ind);

    if (start_ind == end_ind)
    {
        std::cerr << "\nError: No value at tag \"" + tag << "\"!\n" << std::endl;
        throw my_exceptions::tag_error();
    }

    std::string tag_value = config_file.substr(start_ind, end_ind - start_ind);

    // erase whitespaces around the tag value
    // erase whitespace characters before the tag value
    tag_value.erase(0, tag_value.find_first_not_of(inline_whitespaces));

    if (tag_value.length() == 0)
    {
        std::cerr << "\nError: No value at tag \"" + tag << "\"!\n" << std::endl;
        throw my_exceptions::tag_error();
    }
    else
        { tag_value.erase(tag_value.find_last_not_of(inline_whitespaces) + 1); }

    std::cout << "Found: \"" << tag_value << "\"" << std::endl;

    return tag_value;
  // Bouml preserved body end 0002158D

}

/**
 *  The section of the \a config_file between \a section_begin_and_end is searched line by line.
 *  Every not commented line is searched for the three components
 *  tag, equality sign and tag value.
 *  (Which may only be separated by blank and tabulator characters.)
 *
 *  \warning Tags with an equality sign but no value are assigned an empty string as a value.
 *
 *  \bug \a section_begin_and_end could be commented and the function won't notice
 *
 *  \param[in]  section_begin_and_end   The two strings used as markers for the beginning and end of the section.
 *  \param[in]  config_file             The configuration file to be searched.
 *  \param[in]  comment_character       The comment character used in the configuration file.
 *
 *  \exception my_exceptions::tag_error Thrown if neither \a section_begin nor \a section_end could be found.
 *                                      Or if the \a section_end appears before \a section_begin.
 *
 *  \return The tag values mapped to their respective tags.
 *          Prints warnings if uncommented lines inside the section can not
 *          be interpreted as tag and value.
 */
std::multimap < tag, tag_value > configuration::extract_tags_and_values(const std::pair < std::string, std::string > & section_begin_and_end, const std::string & config_file, char comment_character)
{
  // Bouml preserved body begin 0002160D
    std::cout << "Searching for tags in the section at \"" << section_begin_and_end.first << "\"...\n";

    size_t begin_ind = config_file.find(section_begin_and_end.first);
    const size_t END_IND = config_file.find(section_begin_and_end.second);

    if (begin_ind == std::string::npos || END_IND == std::string::npos)
    {
        if(begin_ind == std::string::npos)
        {
            std::cerr << "\nError: Beginning of section \"" << section_begin_and_end.first << "\" not found!\n" << std::endl;
            throw my_exceptions::tag_error();
        }
        else
        {
            std::cerr << "\nError: End of section \"" << section_begin_and_end.first << "\" not found!\n" << std::endl;
            throw my_exceptions::tag_error();
        }
    }

    if (begin_ind > END_IND)
    {
        std::cerr << "\nError: Section \"" << section_begin_and_end.first << "\" ends before it starts!\n" << std::endl;
        throw my_exceptions::tag_error();
    }

    // TODO: get these characters from the current locale somehow
    const std::string inline_whitespaces = " \t";
    const std::string new_line_whitespaces = "\v\f\r\n";

    std::multimap<std::string, std::string> tags_and_values;

    // these two lines skip contiguous line break characters
    size_t new_line_ind = config_file.find_first_of(new_line_whitespaces, begin_ind + section_begin_and_end.first.length());
    new_line_ind = config_file.find_first_not_of(new_line_whitespaces, new_line_ind + 1);

    // begin_ind and new_line_ind shall be the beginning and end indices of the currently searched line
    begin_ind = new_line_ind;
    new_line_ind = config_file.find_first_of(new_line_whitespaces + comment_character, begin_ind);

    // if the section doesn't contain anything, an empty map is returned
    if (new_line_ind > END_IND || new_line_ind == std::string::npos) {}
    else
    {
        while (new_line_ind < END_IND)
        {
            size_t first_char_ind = config_file.find_first_not_of(inline_whitespaces, begin_ind);

            if (first_char_ind == new_line_ind) {}
//                    else if (config_file[first_char_ind] == comment_character) {}
            else if (!std::isalnum(config_file[first_char_ind]))
                { std::cout << "Warning: Ignoring an uncommented line starting with a non-alphanumeric character inside section \"" << section_begin_and_end.first << "\"!" << std::endl; }
            else
            {
                size_t eq_sign_ind = config_file.find_first_of('=', first_char_ind + 1);

                if ( eq_sign_ind > new_line_ind || eq_sign_ind == std::string::npos)
                    { std::cout << "Warning: Ignoring an uncommented line without an equality sign inside section \"" << section_begin_and_end.first << "\"!" << std::endl; }
                else
                {
                    // first_char_ind and last_char_ind shall be the beginning and end_indices of
                    // at first the tag and then the tag value
                    size_t last_char_ind = config_file.find_last_not_of(inline_whitespaces, eq_sign_ind - 1);
                    std::string tag = config_file.substr(first_char_ind, last_char_ind - first_char_ind + 1);

                    first_char_ind = config_file.find_first_not_of(inline_whitespaces, eq_sign_ind + 1);
                    last_char_ind = config_file.find_last_not_of(inline_whitespaces, new_line_ind - 1);

                    std::string value = "";

                    if (last_char_ind - first_char_ind + 1 <= 0)
                        {} //{ std::cout << "Warning: Tag without a value inside section \"" << section_begin_and_end.first << "\"!" << std::endl; }
                    else
                        { value = config_file.substr(first_char_ind, last_char_ind - first_char_ind + 1); }

                    std::cout << "Found tag \"" << tag << "\" with value \"" << value << "\"." << std::endl;
                    tags_and_values.insert({tag, value}); // i could do a move here but i think it isn't necessary...
                }
            }

            new_line_ind = config_file.find_first_of(new_line_whitespaces, new_line_ind); // in case new_line_ind points to a comment character
            new_line_ind = config_file.find_first_not_of(new_line_whitespaces, new_line_ind + 1);
            begin_ind = new_line_ind;
            new_line_ind = config_file.find_first_of(new_line_whitespaces + comment_character, begin_ind);
        }
    }

    return tags_and_values;
  // Bouml preserved body end 0002160D

}


} // namespace input
