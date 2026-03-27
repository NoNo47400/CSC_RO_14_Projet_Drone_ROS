#ifndef _ROS_tourelle_controller_TourelleAngles_h
#define _ROS_tourelle_controller_TourelleAngles_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace tourelle_controller
{

  class TourelleAngles : public ros::Msg
  {
    public:
      typedef float _angle_bottom_type;
      _angle_bottom_type angle_bottom;
      typedef float _angle_middle_type;
      _angle_middle_type angle_middle;
      typedef float _angle_top_type;
      _angle_top_type angle_top;

    TourelleAngles():
      angle_bottom(0),
      angle_middle(0),
      angle_top(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      union {
        float real;
        uint32_t base;
      } u_angle_bottom;
      u_angle_bottom.real = this->angle_bottom;
      *(outbuffer + offset + 0) = (u_angle_bottom.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_angle_bottom.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_angle_bottom.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_angle_bottom.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->angle_bottom);
      union {
        float real;
        uint32_t base;
      } u_angle_middle;
      u_angle_middle.real = this->angle_middle;
      *(outbuffer + offset + 0) = (u_angle_middle.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_angle_middle.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_angle_middle.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_angle_middle.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->angle_middle);
      union {
        float real;
        uint32_t base;
      } u_angle_top;
      u_angle_top.real = this->angle_top;
      *(outbuffer + offset + 0) = (u_angle_top.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_angle_top.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_angle_top.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_angle_top.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->angle_top);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      union {
        float real;
        uint32_t base;
      } u_angle_bottom;
      u_angle_bottom.base = 0;
      u_angle_bottom.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_angle_bottom.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_angle_bottom.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_angle_bottom.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->angle_bottom = u_angle_bottom.real;
      offset += sizeof(this->angle_bottom);
      union {
        float real;
        uint32_t base;
      } u_angle_middle;
      u_angle_middle.base = 0;
      u_angle_middle.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_angle_middle.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_angle_middle.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_angle_middle.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->angle_middle = u_angle_middle.real;
      offset += sizeof(this->angle_middle);
      union {
        float real;
        uint32_t base;
      } u_angle_top;
      u_angle_top.base = 0;
      u_angle_top.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_angle_top.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_angle_top.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_angle_top.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->angle_top = u_angle_top.real;
      offset += sizeof(this->angle_top);
     return offset;
    }

    virtual const char * getType() override { return "tourelle_controller/TourelleAngles"; };
    virtual const char * getMD5() override { return "2e1a1773fcb67db5dcc12ee62a41173b"; };

  };

}
#endif
