/* -*- mode: c++; -*- */
#ifndef __REG_BASE_H__
#define __REG_BASE_H__

class RegBase {
public:
    void setUppercase(bool uppercase) { _uppercase = uppercase; }

protected:
    bool _uppercase;
};

#endif // __REG_BASE_H__
