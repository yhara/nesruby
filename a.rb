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
    end
  else
    if btn_a_pressed(pad0)
      play_sound 3, 0
      dx = 0
      dy = 1
    end
    if y >= goal_y
      dy = 0
      catch_wait -= 1
      if catch_wait == 0
        if x == goal_x
          play_sound 0, 0
          rdy = -1
        end
        y = goal_y
        dy = -1
      end
    end
    if y < start_y
      play_sound 4, 0
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
