#pragma once

#include <tuple>
#include <vector>
#include <string>

namespace gl
{

struct object
{

    constexpr object() noexcept = default;

    constexpr explicit object(GLuint x) noexcept : name(x) { }

    constexpr explicit operator GLuint() const noexcept {
        return name;
    }

    constexpr explicit operator GLuint &() noexcept {
        return name;
    }

    constexpr object(object &&x) noexcept : object(x.name) {
        x.name = 0;
    }

    object &operator=(object &&x) noexcept {
        name = x.name;
        x.name = 0;
        return *this;
    }


protected:
    GLuint name{0};
};


template<GLenum target>
struct basic_texture : object
{
protected:
    basic_texture() noexcept {
        glGenTextures(1, &name);
    }

    ~basic_texture() noexcept {
        if (name != 0)
            glDeleteTextures(1, &name);
    }

public:

    constexpr basic_texture(basic_texture &&) = default;

    basic_texture &operator=(basic_texture &&) = default;

    void bind() noexcept {
        glBindTexture(target, name);
    }

    explicit operator bool() const noexcept {
        return glIsTexture(name) == GL_TRUE;
    }
};


class texture_format_desc final : std::tuple<GLint, GLint, GLint>
{
    using base = std::tuple<GLint, GLint, GLint>;

    enum class ind_ : std::size_t
    {
        internal_format = 0,
        format,
        type
    };

    using base::base;

    template<ind_ i>
    constexpr decltype(auto) get() const noexcept {
        return std::get<static_cast<std::size_t>(i)>(*this);
    }

public:

    constexpr auto internal_format() const noexcept {
        return get<ind_::internal_format>();
    }

    constexpr auto format() const noexcept {
        return get<ind_::format>();
    }

    constexpr auto type() const noexcept {
        return get<ind_::type>();
    }
};

namespace texture_format
{
constexpr texture_format_desc rgb8{GL_RGB8, GL_RGB, GL_BYTE};
constexpr texture_format_desc rgba8{GL_RGBA8, GL_RGBA, GL_BYTE};
constexpr texture_format_desc rgb32f{GL_RGB32F, GL_RGB, GL_FLOAT};
}

struct texture_2d : basic_texture<GL_TEXTURE_2D>
{
    void make_storage
            (
                    GLsizei width,
                    GLsizei height,
                    texture_format_desc tex_fmt,
                    const void *data = nullptr,
                    int level = 0
            ) noexcept {
        glTexImage2D(GL_TEXTURE_2D, level,
                     tex_fmt.internal_format(),
                     width, height,
                     0, tex_fmt.format(),
                     tex_fmt.type(), data);
    }
};

struct texture_3d : basic_texture<GL_TEXTURE_3D>
{
    void make_storage
            (
                    GLsizei width,
                    GLsizei height,
                    GLsizei depth,
                    texture_format_desc tex_fmt,
                    const void *data = nullptr,
                    int level = 0
            ) noexcept {
        glTexImage3D(GL_TEXTURE_3D, level,
                     tex_fmt.internal_format(),
                     width, height, depth,
                     0, tex_fmt.format(),
                     tex_fmt.type(), data);
    }
};

struct render_buffer : object
{
    render_buffer() noexcept {
        glGenRenderbuffers(1, &name);
    }

    ~render_buffer() noexcept {
        if (name)
            glDeleteRenderbuffers(1, &name);
    }

    constexpr render_buffer(render_buffer &&) noexcept = default;

    render_buffer &operator=(render_buffer &&) noexcept = default;

    void make_storage(GLsizei width, GLsizei height, GLenum internal_format) noexcept {
        glRenderbufferStorage(GL_RENDERBUFFER, internal_format, width, height);
    }

    void make_storage(GLsizei samples, GLsizei width, GLsizei height, GLenum internal_format) noexcept {
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, internal_format, width, height);
    }

    void bind() noexcept {
        glBindRenderbuffer(GL_RENDERBUFFER, name);
    }

    void unbind() noexcept {
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }

    explicit operator bool() const noexcept {
        return glIsRenderbuffer(name) == GL_TRUE;
    }

    auto width() const noexcept {
        return get_param<GL_RENDERBUFFER_WIDTH>();
    }

    auto height() const noexcept {
        return get_param<GL_RENDERBUFFER_HEIGHT>();
    }

    auto internal_format() const noexcept {
        return get_param<GL_RENDERBUFFER_INTERNAL_FORMAT>();
    }

    auto red_size() const noexcept {
        return get_param<GL_RENDERBUFFER_RED_SIZE>();
    }

    auto green_size() const noexcept {
        return get_param<GL_RENDERBUFFER_GREEN_SIZE>();
    }

    auto blue_size() const noexcept {
        return get_param<GL_RENDERBUFFER_BLUE_SIZE>();
    }

    auto alpha_size() const noexcept {
        return get_param<GL_RENDERBUFFER_ALPHA_SIZE>();
    }

    auto depth_size() const noexcept {
        return get_param<GL_RENDERBUFFER_DEPTH_SIZE>();
    }

    auto stencil_size() const noexcept {
        return get_param<GL_RENDERBUFFER_STENCIL_SIZE>();
    }

    auto samples() const noexcept {
        return get_param<GL_RENDERBUFFER_SAMPLES>();
    }

private:
    template<GLenum pname>
    GLint get_param() const noexcept {
        GLint value = -1;
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, pname, &value);
        return value;
    }
};

struct frame_buffer : object
{
    frame_buffer() noexcept {
        glGenFramebuffers(1, &name);
    }

    ~frame_buffer() noexcept {
        if (name)
            glDeleteFramebuffers(1, &name);
    }

    constexpr frame_buffer(frame_buffer &&) noexcept = default;

    frame_buffer &operator=(frame_buffer &&) noexcept = default;

    void bind() const noexcept {
        glBindFramebuffer(GL_FRAMEBUFFER, name);
    }

    void unbind() const noexcept {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void attach(const render_buffer &render_target, GLenum attachment) noexcept {
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment,
                                  GL_RENDERBUFFER, static_cast<GLuint>(render_target));
    }

    void attach(const texture_2d &tex2d, GLenum attachment, GLint level = 0) noexcept {
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D,
                               static_cast<GLuint>(tex2d), level);
    }

    explicit operator bool() const noexcept {
        return glIsFramebuffer(name) == GL_TRUE;
    }

    bool is_complete() const noexcept {
        return glCheckFramebufferStatus(GL_FRAMEBUFFER)
               == GL_FRAMEBUFFER_COMPLETE;
    }

    void invalidate(const GLenum *attachemnts, GLsizei num_attachments) noexcept {
        glInvalidateFramebuffer(GL_FRAMEBUFFER, num_attachments, attachemnts);
    }

    void invalidate(const GLenum *attachemnts, GLsizei num_attachments,
                    GLint x, GLint y, GLsizei width, GLsizei height) noexcept {
        glInvalidateSubFramebuffer(GL_FRAMEBUFFER, num_attachments, attachemnts,
                                   x, y, width, height);
    }
};


template<GLenum type>
class basic_shader : public object
{
public:
    basic_shader() noexcept : object(glCreateShader(type)) { }

    ~basic_shader() noexcept {
        if (name)
            glDeleteShader(name);
    }

    constexpr basic_shader(basic_shader &&) noexcept = default;

    basic_shader &operator=(basic_shader &&) noexcept = default;

    explicit operator bool() const noexcept {
        return glIsShader(name) == GL_TRUE;
    }

    void src(const char *const src) noexcept {
        glShaderSource(name, 1, &src, nullptr);
    }

    void compile() noexcept {
        glCompileShader(name);
    }

    std::pair<bool, std::string> status() const {
        GLint compiled = 0;
        glGetShaderiv(name, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infolen = 0;
            glGetShaderiv(name, GL_INFO_LOG_LENGTH, &infolen);
            if (infolen > 1) {
                std::vector<char> infolog(infolen);
                glGetShaderInfoLog(name, infolen, nullptr, infolog.data());
                return {false, std::string{infolog.data(), infolog.size()}};
            }
            return {false, {}};
        }
        else {
            return {true, {}};
        }
    }
};

using vertex_shader = basic_shader<GL_VERTEX_SHADER>;
using fragment_shader = basic_shader<GL_FRAGMENT_SHADER>;


template<GLenum target>
struct basic_buffer : object
{

    basic_buffer() noexcept {
        glGenBuffers(1, &name);
    }

    ~basic_buffer() noexcept {
        if (name) glDeleteBuffers(1, &name);
    }

    constexpr basic_buffer(basic_buffer &&) noexcept = default;

    basic_buffer &operator=(basic_buffer &&) noexcept = default;

    explicit operator bool() const noexcept {
        return glIsBuffer(name) == GL_TRUE;
    }

    void bind() noexcept {
        glBindBuffer(target, name);
    }

    void unbind() noexcept {
        glBindBuffer(target, 0);
    }

    void data(GLsizei size, const GLvoid *data, GLenum usage) noexcept {
        bind();
        glBufferData(target, size, data, usage);
        unbind();
    }

    void sub_data(GLintptr offset, GLsizei size, const GLvoid *data) noexcept {
        bind();
        glBufferSubData(target, offset, size, data);
        unbind();
    }

    void *map(GLintptr offset, GLsizeiptr length, GLbitfield access) noexcept {
        bind();
        return glMapBufferRange(target, offset, length, access);
    }

    void flush_mapped_range(GLintptr offset, GLsizeiptr length) noexcept {
        bind();
        glFlushMappedBufferRange(target, offset, length);
    }

    void unmap() noexcept {
        bind();
        glUnmapBuffer(target);
    }
};

using vertex_buffer = basic_buffer<GL_ARRAY_BUFFER>;
using index_buffer = basic_buffer<GL_ELEMENT_ARRAY_BUFFER>;

struct vertex_array : object
{

    vertex_array() noexcept {
        glGenVertexArrays(1, &name);
    }

    ~vertex_array() noexcept {
        if (name) glDeleteVertexArrays(1, &name);
    }

    constexpr vertex_array(vertex_array &&) noexcept = default;

    vertex_array &operator=(vertex_array &&) noexcept = default;

    explicit operator bool() noexcept {
        return glIsVertexArray(name) == GL_TRUE;
    }

    void bind() noexcept {
        glBindVertexArray(name);
    }

    void unbind() noexcept {
        glBindVertexArray(0);
    }
};


template<typename T>
struct glm_type_traits
{
    static constexpr unsigned size() {
        return sizeof(T);
    }

    static constexpr int gl_type() {
        return GL_FLOAT;
    }

    static void set_uniform(unsigned location, const T &v) {

    }
};


template<>
struct glm_type_traits<glm::vec2>
{
    static constexpr unsigned size() {
        return sizeof(glm::vec2);
    }

    static constexpr int gl_type() {
        return GL_FLOAT;
    }

    static void set_uniform(unsigned location, const glm::vec2 &v) {
        glUniform2fv(location, 1, &v.x);
    }

};

template<unsigned arr_count>
struct glm_type_traits<std::array<glm::vec2, arr_count> >
{
    static constexpr unsigned size() {
        return sizeof(glm::vec2);
    }

    static constexpr int gl_type() {
        return GL_FLOAT;
    }

    static void set_uniform(unsigned location, const std::array<glm::vec2, arr_count> &v) {
        glUniform3fv(location, arr_count, &v[0].x);
    }

};


template<>
struct glm_type_traits<glm::vec3>
{
    static constexpr unsigned size() {
        return sizeof(glm::vec3);
    }

    static constexpr int gl_type() {
        return GL_FLOAT;
    }

    static void set_uniform(unsigned location, const glm::vec3 &v) {
        glUniform3fv(location, 1, &v.x);
    }

};

template<unsigned arr_count>
struct glm_type_traits<std::array<glm::vec3, arr_count> >
{
    static constexpr unsigned size() {
        return sizeof(glm::vec3);
    }

    static constexpr int gl_type() {
        return GL_FLOAT;
    }

    static void set_uniform(unsigned location, const std::array<glm::vec3, arr_count> &v) {
        glUniform3fv(location, arr_count, &v[0].x);
    }

};


template<>
struct glm_type_traits<glm::mat3>
{
    static constexpr unsigned size() {
        return sizeof(glm::mat3);
    }

    static constexpr int gl_type() {
        return GL_FLOAT;
    }

    static void set_uniform(unsigned location, const glm::mat3 &v) {
        glUniformMatrix3fv(location, 1, false, &v[0][0]);
    }

};

template<>
struct glm_type_traits<glm::mat4>
{
    static constexpr unsigned size() {
        return sizeof(glm::mat4);
    }

    static constexpr int gl_type() {
        return GL_FLOAT;
    }

    static void set_uniform(unsigned location, const glm::mat4 &v) {
        glUniformMatrix4fv(location, 1, GL_FALSE, &v[0][0]);
    }

};

struct program : public object
{
    program() : object(glCreateProgram()) { }

    ~program() noexcept {
        if (name)
            glDeleteProgram(name);
    }

    constexpr program(program &&) noexcept = default;

    program &operator=(program &&) noexcept = default;

    explicit operator bool() const noexcept {
        return glIsProgram(name) == GL_TRUE;
    }

    template<GLenum type>
    void attach(basic_shader<type> const &shader) noexcept {
        glAttachShader(name, static_cast<GLuint>(shader));
    }

    template<GLenum type>
    void detach(basic_shader<type> const &shader) noexcept {
        glDetachShader(name, static_cast<GLuint>(shader));
    }

    void link() noexcept {
        glLinkProgram(name);
    }

    std::pair<bool, std::string> link_status() const noexcept {
        return get_status<GL_LINK_STATUS>();
    }

    void validate() noexcept {
        glValidateProgram(name);
    }

    std::pair<bool, std::string> validate_status() const noexcept {
        return get_status<GL_VALIDATE_STATUS>();
    }

    auto uniform_location(const char *uniform_name) noexcept {
        return glGetUniformLocation(name, uniform_name);
    }

    template<typename T>
    void uniform_value(GLint location, const T &value) noexcept {
        glm_type_traits<T>::set_uniform(location, value);
    }

    auto attrib_location(const char *attrib_name) noexcept {
        return glGetAttribLocation(name, attrib_name);
    }

    void bind_attrib_location(const char *attrib_name, GLuint index) noexcept {
        glBindAttribLocation(name, index, attrib_name);
    }

    void bind_attrib_to_buffer(vertex_buffer &buffer, GLuint attrib_index, GLint size, GLenum type, bool normalized,
                               unsigned stride, long unsigned offset = 0) {
        buffer.bind();

        glVertexAttribPointer(attrib_index, size, type, normalized, stride, (void *) offset);

        buffer.unbind();
    }

    void use() const {
        glUseProgram(name);
    }

    void draw_elements(index_buffer &buffer, GLenum mode, GLsizei count, GLenum type, long unsigned offset = 0) {
        buffer.bind();

        glDrawElements(mode, count, type, (GLvoid *) offset);

        buffer.unbind();
    }

    void draw_elements(GLenum mode, GLsizei count, GLsizei first = 0) {

        glDrawArrays(mode, first, count);

    }

    void draw_elements_instanced(index_buffer &buffer, unsigned instances, GLenum mode, GLsizei count, GLenum type,
                                 long unsigned offset = 0) {
        buffer.bind();

        glDrawElementsInstanced(mode, count, type, (GLvoid *) offset, instances);

        buffer.unbind();
    }

private:
    template<GLenum StatusType>
    std::pair<bool, std::string> get_status() const noexcept {
        GLint status{};
        glGetProgramiv(name, StatusType, &status);
        if (!status) {
            GLint infolen = 0;
            glGetProgramiv(name, GL_INFO_LOG_LENGTH, &infolen);
            if (infolen > 1) {
                std::vector<char> infolog(infolen);
                glGetProgramInfoLog(name, infolen, nullptr, infolog.data());
                return {false, std::string{infolog.data(), infolog.size()}};
            }
            return {false, {}};
        }
        else {
            return {true, {}};
        }
    }
};


template<class T>
T fact_func() {
    return {};
}

template<class T>
void test_moveability() {
    T a;
    T b = std::move(a);
    a = std::move(b);
    auto c = fact_func<T>();
    b = std::move(c);
    c.~T();
}

template<class T>
void test_bool() {
    T t;
    if (!t) { }
    else if (t) { }
}

void inline test_interface() {
    test_moveability<object>();
    test_moveability<texture_2d>();
    test_moveability<texture_3d>();
    test_moveability<render_buffer>();
    test_moveability<frame_buffer>();
    test_moveability<vertex_shader>();
    test_moveability<fragment_shader>();
    test_moveability<program>();
    test_moveability<vertex_buffer>();
    test_moveability<index_buffer>();
    test_moveability<vertex_array>();

    test_bool<texture_2d>();
    test_bool<texture_3d>();
    test_bool<render_buffer>();
    test_bool<frame_buffer>();
    test_bool<vertex_shader>();
    test_bool<fragment_shader>();
    test_bool<program>();
    test_bool<index_buffer>();
    test_bool<vertex_buffer>();
    test_bool<vertex_array>();
}


}
