# Introduction
The concept of Black Box is mainly heard by us in case of Aero-planes. Upon a catastrophe the Black Box is used to analyze the root cause of the issue. However, the purpose of Black Box can go beyond catastrophe analysis. Also known as an event data recorder (EDR) or Accident data recorder (ADR) the Black Box can be installed in some automobiles to record various events. These events which are electronically sensed can often pro-actively detect any issue in the vehicle (ex: Engine fault) or monitor the fleet (ex: Drive over speeding), thereby doing pro-active maintenance of the Automotive vehicle. 

By considering today’s busy life, every one wants to reach the destination as soon as possible by ignoring the rules and regulations. By implementing a Black Box which would log critical activities on the car and take appropriate actions in case of rash driving. As mentioned above the root cause of the negligence in the driving would be meeting the daily schedule and go off duty, or to earn extra money by illegal trips etc… So by implementing the mentioned solution it would be easy to keep track of how the vehicle is being used, handled and control the efficiency of the vehicle.

The proposed solution is to log all the critical events like the gear shifts with current speed, the engine temperature, fuel consumption per trip, trip distance etc., The system should allow a password based access to the transport managers to view or download the log to PC if required.

# Requirement Details

### Default Screen:

When the system is in Operation Mode, it would act like a dashboard which would show the current time, speed of vehicle and the latest occurred event.

### Login Screen:

On press of the UP or DOWN (User Keys) keys the system should prompt for password
The password would be the combination of 4 presses (User Keys).
Each press should be denoted a “*” symbol
Every wrong entry would, re prompt for password (Max 3 times for every 15 minutes)
Incomplete key press (pause of 3 seconds) would lead to Default Screen

### Main Menu:

The main menu should contain 2 option
View Log
Set Time
The UP / DOWN keys are used to navigate
A long press of UP Key should enter the selected menu
A long press of DOWN Key should log out
Idle screen for more than 5 secs should log out

### View Log:

Should display all the events captured with log index starting from 0, like
“EVENT NUMBER” “EVENT SIGNATURE” “EVENT TIME” “SPEED AT THE EVENT”
The UP and DOWN key will be used to scroll the entries
Rollover on reaching the max log entries
The system should be live (capture events occurred) even while viewing the log
A long press of UP Key should take you back to main menu

### Set Time

Should show the current time. The Secs field should blink indicating the field to be changed
The UP key should be used to increment the time. Rollover on reaching max
The DOWN key will be used to choose the field.
A long press of UP Key should take you back to main menu

### Event Capture

Required events have to be captured and stored in the memory
Every event should have a format as
“EVENT SIGNATURE” “EVENT TIME” “SPEED AT THE EVENT”
The events should be captured real time, no matter which mode you are in
