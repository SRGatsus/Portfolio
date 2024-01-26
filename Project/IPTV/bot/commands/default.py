from aiogram.types import BotCommandScopeDefault, BotCommandScopeChat, BotCommand

from loader import bot


def get_default_commands() -> list[BotCommand]:
    commands = [
        BotCommand("/start", "Ⓜ️Главное меню"),
        BotCommand("/help", "📱️Контакты"),
    ]

    return commands


async def set_default_commands():
    return


async def set_user_commands(user_id: int):
    return
