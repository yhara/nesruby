y_speed = 2
start_x = 0
start_y = 100
goal_x = 200
goal_y = 200
title = true
show_title
while true
  pad0 = pad_trigger 0
  if title
    if btn_a_pressed(pad0)
      play_sound 3, 0
      hide_title
      title = false

      x = start_x
      y = start_y
      dx = 1
      dy = 0
      rx = goal_x
      ry = goal_y
      rdy = 0
      catch_wait = 30 
      caught = false
    end
  else
    if btn_a_pressed(pad0)
      play_sound 3, 0
      dx = 0
      dy = y_speed
    end
    if y >= goal_y
      dy = 0
      catch_wait -= 1
      if catch_wait == 0
        if goal_x - 8 < x && x < goal_x + 8
          #play_sound 0, 0
          caught = true
          rdy = 0-y_speed
        end
        y = goal_y
        dy = 0-y_speed
      end
    end
    if y < start_y
      if caught
        play_sound 4, 0
      end
      title = true
      show_title
    end
    x += dx
    y += dy
    ry += rdy

    wait_frame
    draw_arrow x, y
    draw_ruby rx, ry
  end
end
