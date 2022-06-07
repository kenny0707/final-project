# final project  

##assemble the bbcar and set up map  
->assemble the bbcar with servos、encoder、laser ping、xbee and QTIs,use a powerbank to supply the power  
->set up a black line map  

##set up mbed  
->select "empty Mbed OS program" under MBED OS 6 Enter final project for Program name. Check "Make this the active program" (default). Under "Mbed OS Location"  
,check "Link to an existing shared Mbed OS instance" and select "~/Mbed Programs/mbed01/mbed-os/". This will reuse Mbed OS in mbed01/. Click "Add Program"  
->copy main to main.cpp  
->add library [bbcar](https://gitlab.larc-nthu.net/ee2405_2022/bbcar.git) and [erpc](https://gitlab.larc-nthu.net/ee2405_2022/erpc_c.git) with main to mbed  

##erpc generate cpp and h hile for mbed  
->use erpcgen with bbcar_control.erpc to generate codes  
->copy controller_server.cpp,controller_server.h,controller.h to mbed  

##start running bbcar  
->run final project into bbcar  
->put the car on the origin and press reset  
->bbcar should start running on the map  

##use python to send remote call to mbed  
->open bbcar_control_client.py in bbcar_erpc directory with git bash and then type cd ~/Mbed\ Programs/final_project and then enter  
and then type python bbcar_control_client.py <your serial port>   
->type i,mbed should start publish bbcar information   
