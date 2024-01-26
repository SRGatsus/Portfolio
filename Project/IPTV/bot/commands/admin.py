from aiogram.types import BotCommandScopeChat, BotCommand

from loader import bot
from .default import get_default_commands


def get_admin_commands() -> list[BotCommand]:
    commands = get_default_commands()

    commands.extend(
        [
            BotCommand("/admin", "⭐Админ"),
        ]
    )

    return commands


async def set_admin_commands(user_id: int):
    return


async def delete_admin_commands(user_id: int):
    return
