y_speed = 2
x_speed = 5
sx = 0
sy = 100
gx = 200
gy = 200
title = true
show_title
while true
  pad0 = pad_trigger 0
  if title
    if btn_a_pressed(pad0)
      play_sound 3, 0
      hide_title
      title = false
      #play_music 0

      x = sx
      y = sy
      dx = x_speed
      dy = 0
      rx = gx
      ry = gy
      rdy = 0
      cw = 30 
      caught = false
    end
  else
    x += dx
    y += dy
    ry += rdy
    if x >= 255
      dx = 0-x_speed
    elsif x < 0
      x = 0
      dx = x_speed
    end
    if y == sy && btn_a_pressed(pad0)
      #stop_music
      play_sound 3, 0
      dx = 0
      dy = y_speed
    end
    if y >= gy
      dy = 0
      cw -= 1
      if cw == 0
        if gx - 8 < x && x < gx + 8
          #play_sound 0, 0
          caught = true
          rdy = 0-y_speed
        end
        y = gy
        dy = 0-y_speed
      end
    end
    if y < sy
      if caught
        play_sound 4, 0
      end
      title = true
      show_title
    end

    wait_frame
    draw_arrow x, y
    draw_ruby rx, ry
  end
end
