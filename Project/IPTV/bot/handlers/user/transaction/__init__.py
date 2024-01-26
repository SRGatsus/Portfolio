from aiogram import Dispatcher

from .list_transactions import list_transactions
from .view_transaction import view_transaction
from .. import user_info
from ....states.user import UserStates


def setup(dp: Dispatcher):
    dp.register_callback_query_handler(list_transactions, lambda c: str(c.data).startswith( "list_transaction"),
                                       state=[UserStates.user_info,UserStates.list_transaction])
    dp.register_message_handler(view_transaction, content_types=["text"],
                                state=[UserStates.list_transaction])
    dp.register_callback_query_handler(user_info, lambda c: c.data == "back", state=UserStates.list_transaction)
    dp.register_callback_query_handler(list_transactions, lambda c: c.data == "back", state=UserStates.view_transaction)
