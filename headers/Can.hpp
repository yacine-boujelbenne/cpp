#ifndef _CAN_
#define _CAN_

#include <cstdint>
#include <vector>
#include <string>

class Can {
public:
    Can();
    Can(uint32_t id, const std::vector<uint8_t> &data);
    void print();
    void setFrameType(bool extended) { extendedFrame_ = extended; }
    bool isExtendedFrame() const { return extendedFrame_; }
    void setId(uint32_t id) { id_ = id; }
    void setData(std::vector<uint8_t> data) { data_ = data; }
    uint32_t getId() const { return id_; }
    const std::vector<uint8_t>& getData() const { return data_; }
    
    static std::vector<uint8_t> encoder(const std::string& data) {
        return std::vector<uint8_t>(data.begin(), data.end());
    }
    
    static std::string decoder(const std::vector<uint8_t>& data) {
        return std::string(data.begin(), data.end());
    }

private:
    bool extendedFrame_ = false;
    uint32_t id_ = 0;
    std::vector<uint8_t> data_;
};

#endif