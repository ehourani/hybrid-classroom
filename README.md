# hybrid-classroom-server
Hybrid classroom server for the 6.810 final project!



## URLs

Navigate to https://hybrid-classroom.herokuapp.com and add the following URL endings based on your intended functionality:

| Path ending              | Functionality                                                |
| ------------------------ | ------------------------------------------------------------ |
| /                        | Home page!                                                   |
| /session                 | Creates a new hybrid classroom                               |
| /current-session         | Displays information about the current hybrid classroom      |
| /reset                   | Deletes all hybrid classrooms created thus far               |
| /bear                    | Gives information about the bear in the form: `<hand_raised>; (<r>, <g>, <b>)` |
| /color                   | Get the bear's current color in the form (<r>, <g>, <b>)     |
| /color?r=<r>&g=<g>&b=<b> | Sets the bear's color to RGB (<r>, <g>, <b>). e.g. `/color?r=40&g=100&b=255` sets it to (40, 100, 255) |
| /understand              | Displays the student's current understanding level (True, False, None) |
| /understand?good=true    | Sets the student's understanding to True (i.e. if student pushed thumbs up button) |
| /understand?good=false   | Sets the student's understanding to False (i.e. if student pushed thumbs down button) |
| /understand?good=none    | Resets the student's understanding level                     |
| /raisehand               | Toggles student's raised hand (1st push = raised, 2nd push = lowered) |
| /applause                | Displays the student's current applause status               |
| /applause?active=true    | Activates student's applause                                 |
| /applause?active=false   | Deactivates student's applause                               |
| /teacher-highfive        | Initiates a high five from the teacher                       |
| /student-highfive        | Student responds to a high five                              |

