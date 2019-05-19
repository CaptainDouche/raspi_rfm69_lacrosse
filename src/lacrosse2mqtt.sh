#!/bin/sh

# create a named pipe where the mqtt_pub will read from
MQFIFO=/tmp/mqfifo
test -p ${MQFIFO} || mkfifo ${MQFIFO}

/bin/rfm69_lacrosse --format "%x/%t=%v %u\n" --output ${MQFIFO} --daemon
/bin/mqtt_cli --publish "sensors/" --delim "=" --input ${MQFIFO} --daemon

