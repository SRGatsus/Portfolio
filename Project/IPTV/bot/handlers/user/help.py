from aiogram import types


async def bot_help(msg: types.Message):
    await msg.answer(
        """<b>С вопросами, пожеланиями, жалобами и предложениями сюда</b> - @telegram_nick\n""",
        disable_web_page_preview=True,
    )
