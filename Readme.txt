ECEN 5229 Computer Graphics Summer 2018 
Final Project: Arcade games
Author       : Shekhar
Reference    : The base code and helper C-files are taken and modified from Prof Willem A. (Vlakkies) Schreüder example codes and a lot of online support/reference from many different websites


Arcade games: 
1. Car Driving Licese test game 
2. Bowling alley game
When you exeute the game, you will see a menu page. In this menu page you can press Space to continue with 1st game or press @ to go to 2nd game. 
The Car game also has a menu, here you can select which environment you want to drive in Desert/Denver by pressing 1/2 respectively. The selected option will be shown by a "(Selected)" tag beside the environment option. 


Build and Run

1. Extract the project folder
2. make clean
3. make
4. ./finalProject

#Common control 
  
  @                       To toggle between the games
  %                       To switch on/off Seizing the Resize window ( you can block/unblock the manual resize)

#Car Game Controls:
  l          		  Toggles lighting
  a/A        		  Decrease/increase ambient light
  d/D        		  Decrease/increase diffuse light
  s/S        		  Decrease/increase specular light
  e/E        		  Decrease/increase emitted light
  n/N        		  Decrease/increase shininess
  m          		  Toggles light movement
  +/-        		  Change field of view of perspective
  PgDn/PgUp  		  Zoom in and out
  0          		  Reset view angle
  ArrowKey Up        	  Move Car Forward
  ArrowKey Down        	  Move Car Back
  ArrowKey Left        	  Move Car Left
  ArrowKey Right     	  Move Car Right
  r/R            	  Restart the Game
  z                       To increase speed of car 
  x                       To decrease speed of the car 

  #rain/hail
  !                       To rain
  h                       To hail
  #                       To stop raining 

  ESC                	  Exit the game

#Bowling Game Controls:

  left/right arrow-keys    To rotate view around the x axis
  up/down arrow-keys 	   To rotate view and the y axis
  pgDn/ pgUp 		   To zoom out and in
  p 			   To change the projection mode
  0 			   resets the view
  l                        toggles lighting
  1 to 8 		   To launch the bowling ball
  -/+ 			   decrease/increase the field of view
  [/] 			   decreases/increases the light y-component
  f 			   To move forward (in first person mode only)
  v			   To move backward (in first person mode only)
  R 			   To roll the ball in first person

Total Number of Hours: 80+ 

Why do I deserve an A? 
1. I have achieved my super strech goal of completing both the games. 
2. Even though I had a lot of help from stackoverflow, openglblogs, someUniversity notes; none of them worked for what I was looking for. Thus based on that I started to build both the projects. 
3. As a graduate project, I implemented Collision Detection, Special Cone shape (not a normal solid cone - it is a fancy spiral strip of a cone with appropriate lighthing), Trees, Houses, Skybox (with chainging environment depening on user input), Two different menu-options, different environment selections, multiple screen handling, buildings and unique numeroud key controls.
For second game I had guidence from many online opengl blogs, but I had to implement them on my own. To design the dec, to design the bowling pins, explosion and reset of pins, first-person-perspective to rotate the entire view of the bowling game till the appropriate zpos, drawling lanes, drawing pyramids, 8 alleies, the ceiling with dancing lights, ball return logic etc..  
All the references are mentioned in References.txt 


