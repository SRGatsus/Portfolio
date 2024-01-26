from aiogram import types
from aiogram.dispatcher import FSMContext

from utils.misc.logging import logger


async def log_any_error(update: types.Update, exception: Exception):
    logger.error(f'Ошибка при обработке запроса {update}: {exception}')
