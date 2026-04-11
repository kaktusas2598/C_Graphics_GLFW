if input.isKeyPressed(81) then -- Q
    print("Pressed Q")
end

-- Variable reset everytime script is called
v = 5

-- Persistent variable
p = p or 0

-- Called each frame
function update()
    if input.isKeyDown(87) then print 'W' end
    if input.isKeyDown(83) then print 'S' end
    if input.isKeyDown(65) then print 'A' end
    if input.isKeyDown(68) then print 'D' end
    p = p + 1
    --print('p: '..p)
    --print('v: '..v)
    local x, y, z = camera.getPosition()
    print(x,y,z)
end
