#!/bin/bash

DIR=$(dirname "$0")
mkdir -p $DIR/../../game/serialization/
flatc --cpp --python -o $DIR/../../game/serialization/ -b $DIR/ScoreItem.fbs $DIR/ScoreLog.fbs $DIR/ScorePlayer.fbs
