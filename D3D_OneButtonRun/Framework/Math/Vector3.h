#pragma once

class Vector3
{
public:
    Vector3(Float3 value)
        : m_value(XMLoadFloat3(&value))
    {
    }
    Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f)
        : m_value(XMVectorSet(x, y, z, 0))
    {
    }
    Vector3(Vector4 value) : m_value(value)
    {
    }

    void SetX(const float& x)
    {
        m_value = XMVectorSetX(m_value, x);
    }
    void SetY(const float& y)
    {
        m_value = XMVectorSetY(m_value, y);
    }
    void SetZ(const float& z)
    {
        m_value = XMVectorSetZ(m_value, z);
    }

    float GetX() const { return XMVectorGetX(m_value); }
    float GetY() const { return XMVectorGetY(m_value); }
    float GetZ() const { return XMVectorGetZ(m_value); }

    __declspec(property(get = GetX, put = SetX)) float x;
    __declspec(property(get = GetY, put = SetY)) float y;
    __declspec(property(get = GetZ, put = SetZ)) float z;

    operator Vector4() { return m_value; }
    operator Float3()
    {
        Float3 result;
        XMStoreFloat3(&result, m_value);

        return result;
    }

    Vector3 operator+(const Vector3& v) const { return m_value + v.m_value; }
    Vector3 operator-(const Vector3& v) const { return m_value - v.m_value; }
    Vector3 operator*(const Vector3& v) const { return m_value * v.m_value; }
    Vector3 operator/(const Vector3& v) const { return m_value / v.m_value; }

    Vector3 operator*(const float& s) const { return m_value * s; }
    Vector3 operator/(const float& s) const { return m_value / s; }

    void operator+=(const Vector3& v) { m_value += v.m_value; }
    void operator-=(const Vector3& v) { m_value -= v.m_value; }
    void operator*=(const Vector3& v) { m_value *= v.m_value; }
    void operator/=(const Vector3& v) { m_value /= v.m_value; }

    void operator*=(const float& s) { m_value *= s; }
    void operator/=(const float& s) { m_value /= s; }
    float operator[](const UINT& index) const
    {
        switch (index)
        {
        case 0:
            return GetX();
            break;
        case 1:
            return GetY();
            break;
        case 2:
            return GetZ();
            break;
        }
        return 0.0f;
    }

    friend void operator+=(Float3& v1, const Vector3& v2)
    {
        Vector4 temp = XMLoadFloat3(&v1) + v2.m_value;

        XMStoreFloat3(&v1, temp);
    }

    friend void operator-=(Float3& v1, const Vector3& v2)
    {
        Vector4 temp = XMLoadFloat3(&v1) - v2.m_value;

        XMStoreFloat3(&v1, temp);
    }

    friend void operator*=(Float3& v1, const Vector3& v2)
    {
        Vector4 temp = XMLoadFloat3(&v1) * v2.m_value;

        XMStoreFloat3(&v1, temp);
    }

    friend void operator/=(Float3& v1, const Vector3& v2)
    {
        Vector4 temp = XMLoadFloat3(&v1) / v2.m_value;

        XMStoreFloat3(&v1, temp);
    }

    bool operator==(const Vector3& v)
    {
        return XMVector3Equal(m_value, v.m_value);
    }
    bool operator!=(const Vector3& v)
    {
        return !XMVector3Equal(m_value, v.m_value);
    }

    Vector3 GetNormalized() const { return XMVector3Normalize(m_value); }
    void Normalize() { m_value = XMVector3Normalize(m_value); }
    float Length() const { return XMVectorGetX(XMVector3Length(m_value)); }

    Vector4* GetValue() { return &m_value; }
    static Vector3 Zero() { return Vector3(); }
    static Vector3 One() { return Vector3(1,1,1); }

private:
    Vector4 m_value;
};