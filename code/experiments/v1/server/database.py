############################################################
#
# Database
#
############################################################
import datetime
import peewee

db = peewee.SqliteDatabase('data_server.db')


class Memory(peewee.Model):
    id = peewee.PrimaryKeyField()
    data = peewee.BlobField()
    generation = peewee.IntegerField()
    date = peewee.DateTimeField(default=datetime.datetime.now)

    class Meta:
        database = db


class Weights(peewee.Model):
    id = peewee.PrimaryKeyField()
    data = peewee.BlobField()
    generation = peewee.IntegerField()
    date = peewee.DateTimeField(default=datetime.datetime.now)

    class Meta:
        database = db


class GameResult(peewee.Model):
    id = peewee.PrimaryKeyField()
    game_id = peewee.TextField()
    generation = peewee.IntegerField()
    checksum = peewee.TextField()
    loss = peewee.DoubleField()
    score = peewee.IntegerField()
    victory = peewee.BooleanField()
    date = peewee.DateTimeField(default=datetime.datetime.now)

    class Meta:
        database = db

try:
    db.create_tables([GameResult, Memory, Weights])
except peewee.OperationalError:
    pass
