glfw_context_version_major = 3
glfw_context_version_minor = 3

window = {
    title     = 'Lua 2D Game Engine',
    width     = 500,
    height    = 300,
    x         = 100,
    y         = 100,
    icon_path = './img/icon.bmp'
}

color = {
    red   = 0.5,
    green = 0.5,
    blue  = 1.0
}

keys = {
    ESC = 256
}

main = function()
    glfw_context_init (glfw_context_version_major, glfw_context_version_minor)
    glfw_create_window(window)

    --[[
    print(read_file('./glsl/vertex.glsl'))
    print(read_file('./glsl/fragment.glsl'))
    --]]

    FPS           = 60.0
    last_frame    = 0.0
    current_frame = 0.0

    while not glfw_window_should_close() do
        current_frame = glfw_get_time()

        if (current_frame - last_frame) > (1.0 / FPS) then
            if glfw_get_key(keys.ESC) then
                glfw_set_window_should_close()
            end

            gl_clear_buffers (color)
            glfw_swap_buffers()
            glfw_poll_events ()

            last_frame = current_frame
        end
    end

    glfw_context_terminate()
end
