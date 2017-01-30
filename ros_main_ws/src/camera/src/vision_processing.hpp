#ifndef vision_processing_HPP
#define vision_processing_HPP
#include "includes.ihh"

class qr_hunter
{
public:

    qr_hunter(std::vector<cv_detect::qr> items)
    {
        for (const auto item : items){
            if (item.data == "http://robotics.ortelio.co.uk") {
                qr = item;
            }
        }
    }
    
    /// determine relative position
    std::string relative_position()
    {
        // if any of the right points is on the left size => left 
        if (qr.top_right.x < 160 || qr.bot_right.x < 160) {
            left = true;
            return to_json();
        }
        // if any of the left points is on the right size => right
        else if (qr.top_left.x > 480 || qr.bot_left.x > 480) {
            right = true;
            return to_json();
        }
        // if left points in left, and right points in centre => left
        else if (qr.top_left.x < 160 && qr.top_right.x < 320) {
            left = true;
            return to_json();
        }
        else if (qr.bot_left.x < 160 && qr.bot_right.x < 320) {
            left = true;
            return to_json();
        }
        // if right points in right and left poitns in centre => right
        else if (qr.top_right.x > 480 && qr.top_left.x > 320) {
            right = true;
            return to_json();
        }
        else if (qr.bot_right.x > 480 && qr.bot_left.x > 320) {
            right = true;
            return to_json();
        }
        else {
            centre = true;
            return to_json();
        }
    }   

    // TODO: determine distance by size of QR
 
private:

    std::string to_json()
    {
        nlohmann::json j = {{"left", left}, {"right", right}, {"centre", centre}};
        return j.dump();
    }
    
    cv_detect::qr qr;
    bool left, right, centre = false;
};

#endif
