flatc --cpp --python -b ScoreItem.fbs ScoreLog.fbs ScorePlayer.fbs
mkdir -p ../../game/serialization/
mv ./*.h ../../game/serialization/
