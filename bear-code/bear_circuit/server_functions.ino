void check_server(char delimiter) {
  char* pch;
  char* delim = &delimiter;
  int count = 0;
  char body[30];
  sprintf(body, "", " ");
  int body_len = strlen(body); //calculate body length (for header reporting)
  sprintf(request_buffer, "GET /bear HTTP/1.1\r\n");
  strcat(request_buffer, "Host: hybrid-classroom.herokuapp.com\r\n");
  strcat(request_buffer, "\r\n"); //new line from header to body
  //  strcat(request_buffer, body); //body
  strcat(request_buffer, "\r\n"); //header
  strcat(request_buffer, "\r\n"); //header
  //  Serial.println(request_buffer);
  do_http("hybrid-classroom.herokuapp.com", request_buffer, response_buffer, OUT_BUFFER_SIZE, RESPONSE_TIMEOUT, true);
  //  Serial.println(response_buffer);
  pch = strtok(response_buffer, delim);
  //  Serial.println("STARTING PARSING");
  if (*pch == 'T') {
    hand_raised = true;
  } else {
    hand_raised = false;
  }
  while (pch != NULL) {
    pch = strtok(NULL, delim);
    if (count < 3) { //color
      color[count] = atoi(pch);
    }
    else if (count == 3) { //understanding
      if (*pch == 'T') {
        color[0] = 0;
        color[1] = 255;
        color[2] = 0;
      } else if (*pch == 'F') {
        color[0] = 255;
        color[1] = 0;
        color[2] = 0;
      }
    }
    else if (count == 4) { //applause
      if (*pch == 'T') {
        applause_state = 1;
      } else if (*pch == 'F') {
        applause_state = 0;
      }
    }
    count ++;
  }
}

void feedback(char* type) {
  //  Serial.println("------------------------------------------server get----------------------------");
  char body[40];

  sprintf(body, "", " ");
  int body_len = strlen(body); //calculate body length (for header reporting)
  sprintf(request_buffer, "GET /%s HTTP/1.1\r\n", type);
  strcat(request_buffer, "Host: hybrid-classroom.herokuapp.com\r\n");
  strcat(request_buffer, "\r\n"); //new line from header to body
  do_http("hybrid-classroom.herokuapp.com", request_buffer, response_buffer, OUT_BUFFER_SIZE, RESPONSE_TIMEOUT, true);
  if (response_buffer[0] == 'F') {
    touch_state_1 = 2;
  }
}
