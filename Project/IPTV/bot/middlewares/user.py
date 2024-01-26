from aiogram.dispatcher.handler import CancelHandler
from aiogram.dispatcher.middlewares import BaseMiddleware
from aiogram.types import Message, CallbackQuery, InlineQuery, ChatJoinRequest

from services.users import get_or_create_user


class UsersMiddleware(BaseMiddleware):
    @staticmethod
    async def on_process_message(message: Message, data: dict[str]):
        if "channel_post" in message or message.chat.type != "private":
            raise CancelHandler()
        await message.answer_chat_action("typing")
        user = message.from_user
        user = get_or_create_user(user.id, user.username)
        if user.is_blocked:
            await message.answer("Вас заблокировали в данном боте, обратитесь в поддержку")
            raise CancelHandler()
        data["user"] = user

    @staticmethod
    async def on_process_callback_query(callback_query: CallbackQuery, data: dict[str]):
        user = callback_query.from_user

        user = get_or_create_user(user.id, user.username)
        if user.is_blocked:
            await callback_query.message.answer("Вас заблокировали в данном боте, обратитесь в поддержку")
            raise CancelHandler()
        data["user"] = user

    @staticmethod
    async def on_process_inline_query(inline_query: InlineQuery, data: dict[str]):
        user = inline_query.from_user

        user = get_or_create_user(user.id, user.username)
        if user.is_blocked:
            await inline_query.answer("Вас заблокировали в данном боте, обратитесь в поддержку")
            raise CancelHandler()
        data["user"] = user

    @staticmethod
    async def on_process_chat_join_request(update: ChatJoinRequest, data: dict[str]):
        user = update.from_user

        user = get_or_create_user(user.id, user.username)
        if user.is_blocked:
            raise CancelHandler()
        data["user"] = user
