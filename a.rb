x = 123
y = 100
dx = 1
dy = 0
goal_x = 200
goal_y = 200
rx = goal_x
ry = goal_y
while true
  i = pad_trigger 0
  if btn_a_pressed(i)
    play_sound 3, 3
    dx = 0
    dy = 1
  end
  x += dx
  y += dy

  wait_frame
  draw_arrow x, y
  draw_ruby rx, ry
end
