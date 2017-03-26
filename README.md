# Bài tập lớn môn đồ họa máy tính

## Coding convention
### Tên biến
- Đặt tên biến có ý nghĩa bằng tiếng Anh
- Đặt tên biến theo kiểu `lowerCamelCase`
- Biến static thêm tiền tố `s_`. Ví dụ `s_variable`
- Hạn chế biến global, nếu phải dùng thêm tiền tố `g_`. Ví dụ `g_variable`
- Member của một class thêm tiền tố `m_`. Ví dụ `m_variable`
### Tên hàm
- Đặt tên hàm theo kiểu `UpperCamelCase`
- Tên hàm nên là một động từ
- Cố gắng mỗi hàm làm một và chỉ một nhiệm vụ => dễ đặt tên hàm, code trong sáng
### Tên hằng
- Tên hằng theo kiểu `UPPERCASE_WITH_UNDERSCORES`
- Hạn chế hard code các giá trị, sử dụng hằng hoặc `#define`
### Tên class
- Đặt tên class theo kiểu `UpperCamelCase`
### Layout
```C++
int main()
{
    while (true)
    {
        // statements
        if (conditions)
        {
            // statements
        }
        else if (conditions)
        {
            // statements
        }
    }
}
```


