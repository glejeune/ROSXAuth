require '../lib/rosxauth'

h = ROSXAuth.new()
if( h.auth == ROSXAuth::ErrAuthorizationSuccess )
  puts "Success !"
else
  puts "No, you can continue !"
end

if h.auth? 
  puts "C'est tout bon !!!"
end

f = h.exec( "/bin/ls", "-l" )
if f.nil?
  puts "i said NO !!!"
else
  IO.for_fd( f ).each do | g |
    g.each_line { | l | puts l }
  end
end

f = h.exec( "/usr/bin/touch", "pipo.txt" )
if f.nil?
  puts "i said NO !!!"
else
  IO.for_fd( f ).each do | g |
    g.each_line { | l | puts l }
  end
end

hh = ROSXAuth.new()

f = hh.exec( "/bin/ls", ["-l", "-a"] )
if f.nil?
  puts "i said NO !!!"
else
  IO.for_fd( f ).each do | g |
    g.each_line { | l | puts l }
  end
end

f = hh.exec( "/bin/ls", [self, "-l"] )
