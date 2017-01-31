from ai.RemoteAI.RemoteClient import RemoteClient
import log
logger = log.setup_custom_logger('root')


async def on_event():
    print("lL")


if __name__ == '__main__':
    RemoteClient(on_event=on_event)
