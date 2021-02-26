const char * const heart_emoji ="\
    000,255,000,255,000\n\
    255,255,255,255,255\n\
    255,255,255,255,255\n\
    000,255,255,255,000\n\
    000,000,255,000,000\n";

    MicroBitImage img_heart(heart_emoji);

const char *const d_emoji ="\
    255,000,000,000,255\n\
    000,255,000,255,000\n\
    000,000,000,000,000\n\
    255,255,255,255,255\n\
    255,000,255,000,255\n";
MicroBitImage spaceInvader(d_emoji);

const char *const s_heart = "\
    000,000,000,000,000\n\
    000,255,000,255,000\n\
    000,255,255,255,000\n\
    000,000,255,000,000\n\
    000,000,000,000,000\n";
MicroBitImage smallHeart(s_heart);

const char *const b_heart = "\
    000,255,000,255,000\n\
    255,255,255,255,255\n\
    255,255,255,255,255\n\
    000,255,255,255,000\n\
    000,000,255,000,000\n";
MicroBitImage heart(b_heart);

const char *const happy_emoji = "\
    000,255,000,255,000\n\
    000,255,000,255,000\n\
    000,000,000,000,000\n\
    255,000,000,000,255\n\
    000,255,255,255,000\n";
MicroBitImage happy(happy_emoji);

const char *const sad_emoji ="\
    000,255,000,255,000\n\
    000,255,000,255,000\n\
    000,000,000,000,000\n\
    000,255,255,255,000\n\
    255,000,000,000,255\n";
MicroBitImage sad(sad_emoji);

const char *const angry_emoji = "\
    255,000,000,000,255\n\
    000,255,000,255,000\n\
    000,000,000,000,000\n\
    255,255,255,255,255\n\
    255,000,000,000,255\n";
MicroBitImage angry(angry_emoji);

const char * const asleep_emoji = "\
    000,000,000,000,000\n\
    255,255,000,255,255\n\
    000,000,000,000,000\n\
    000,255,255,255,000\n\
    000,000,000,000,000\n";
MicroBitImage asleep(asleep_emoji);

const char *surprised_emoji = "\
    000,255,000,255,000\n\
    000,000,000,000,000\n\
    000,000,255,000,000\n\
    000,255,000,255,000\n\
    000,000,255,000,000\n";
MicroBitImage surprised(surprised_emoji);

const char *correct_emoji = "\
    000,000,000,000,000\n\
    000,000,000,000,255\n\
    000,000,000,255,000\n\
    255,000,255,000,000\n\
    000,255,000,000,000\n";
MicroBitImage yes(correct_emoji);

const char *wrong_emoji = "\
    255,000,000,000,255\n\
    000,255,000,255,000\n\
    000,000,255,000,000\n\
    000,255,000,255,000\n\
    255,000,000,000,255\n";
MicroBitImage no(wrong_emoji);

const char * const arrow_left_emoji ="\
    000,000,255,000,000\n\
    000,255,000,000,000\n\
    255,255,255,255,255\n\
    000,255,000,000,000\n\
    000,000,255,000,000\n";
MicroBitImage arrow_left(arrow_left_emoji);

const char * const arrow_right_emoji ="\
    000,000,255,000,000\n\
    000,000,000,255,000\n\
    255,255,255,255,255\n\
    000,000,000,255,000\n\
    000,000,255,000,000\n";
MicroBitImage arrow_right(arrow_right_emoji);   

const char * const tick_emoji ="\
    000,000,000,000,000\n\
    000,000,000,000,255\n\
    000,000,000,255,000\n\
    255,000,255,000,000\n\
    000,255,000,000,000\n";
MicroBitImage tick(tick_emoji);  

const char * const wink_emoji ="\
    000,255,000,000,000\n\
    000,000,000,000,000\n\
    255,000,000,000,255\n\
    000,255,255,255,000\n\
    000,000,000,000,000\n";
MicroBitImage wink(wink_emoji);  

const MicroBitImage PICTURES[] = {happy, heart, smallHeart, sad, angry, asleep, surprised, yes, no};
const int storedPictures = 9;