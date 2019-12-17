#ifndef INPUT_TAG_ID_H
#define INPUT_TAG_ID_H


namespace input {

/// There is one ID for each tag and for each tag section in the configuration file.
/// The IDs are used internally to refer to the tags.
enum tag_id
{
    INPUT_DIR,
    OUTPUT_FILE,
    OUTPUT_FILE_NAME,
    CHANNEL,
    TIME,
    TIME_STRING,
    COLD_BATH,
    WARM_BATH,
    DATA,
    DATA_ORDER,
    INT_NODES,
    REAL_NODES,
    STRING_NODES,
    C_B_BEGIN,
    W_B_BEGIN,
    C_B_END,
    W_B_END,
    MAPPING,
    SIMPLE_CALIB,
    SINGLE_END_CALIB
};

} // namespace input

#endif
