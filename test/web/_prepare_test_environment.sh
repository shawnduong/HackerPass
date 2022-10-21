#!/bin/sh
# Prepare a test environment with:
# 3 users
# - 0xdeadbeef
# - 0xdecafbad
# - 0xfeedc0de
# 2 events
# - "Cult Offerings 101" (500)
#   - Learn how to make sacrifices to old gods.
#   - COB2 110
#   - Lucy
#   - November 12, 2022 @ 13:00 -> 15:00 (Pacific)
# - "How to Install Linux on a Dead Badger" (250)
#   - Learn how to make Linux run on anything.
#   - COB2 120
#   - Anonymous
#   - November 12, 2022 @ 14:00 -> 15:00 (Pacific)
# 1 pre-existing attendance
# - 0xdeadbeef -> "Cult Offerings 101"

./create_user.sh 3735928559
./create_user.sh 3737844653
./create_user.sh 4276994270

./create_event.sh 500 "Cult Offerings 101" "Learn how to make sacrifices to old gods." "COB2 110" "Lucy" 1668286800 1668294000
./create_event.sh 250 "How to Install Linux on a Dead Badger" "Learn how to make Linux run on anything." "COB2 120" "Anonymous" 1668290400 1668294000

./create_attendance.sh 3735928559 1
