flatc --cpp --python -b ScoreItem.fbs ScoreLog.fbs ScorePlayer.fbs
mv ./*.h ../../game/serialization/
