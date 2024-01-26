import datetime

from aiogram import types
from aiogram.dispatcher import FSMContext

import keyboard
from bot.handlers.admin.tariff.view import view
from bot.states.admin import AdminStates
from loader import bot, db_session
from models import User, Tariff, Transaction
from models.transaction import typesTransaction
from utils.misc.logging import logger
from ..every import every
from ..view import view


async def init_input(callback_query: types.CallbackQuery, state: FSMContext, user: User):
    try:
        data = await state.get_data()
        if not user.is_admin:
            info_text = f"Вы не админ"
            msg = await callback_query.message.answer(info_text, reply_markup=keyboard.state_keyboard(is_main_menu=True,
                                                                                                      is_back=False))
            await state.update_data({"last_msg": msg.message_id})
            return
        ct_state = callback_query.data.split(";")[1]
        info_text = ""
        flag = True
        reply_markup = keyboard.state_keyboard(is_back=True)
        page_size = 50

        if ct_state == "add_tariff":
            if len(callback_query.data.split(";")) > 1:
                page = callback_query.data.split(";")[-1]
                page = int(page)
            else:
                page = 1
            offset_data = page_size * (page - 1)
            tariffs = db_session.query(Tariff).order_by(Tariff.id.asc()).offset(offset=offset_data).limit(
                limit=page_size).all()
            info_text = (f"Обозначения:\n"
                         f"🟢 - тариф активен\n"
                         f"🟡 - тариф неактивен, но есть пользователь пользующийся им\n"
                         f"🔴 - тариф неактивен\n"
                         f"Список тарифов:\n")
            if len(tariffs) == 0:
                info_text += f"Пусто\n"
            else:
                for tariff in tariffs:
                    if tariff.is_active:
                        info_text += f"🟢 {tariff.id}) {tariff.title}\n"
                    else:
                        is_user = db_session.query(User).filter(User.tariff_id == tariff.id).count()
                        if is_user > 0:
                            info_text += f"🟡 {tariff.id}) {tariff.title}\n"
                        else:
                            info_text += f"🔴 {tariff.id}) {tariff.title}\n"
                reply_markup = keyboard.pagination_keyboard(data_count=len(tariffs),
                                                            data_max=page_size,
                                                            prefix="u_change;add_tariff",
                                                            page=page
                                                            )
            info_text += ("Введите номер тарифа,который желаете добавить:")
            await state.update_data({"cu_state": "cu_add_tariff"})
        elif ct_state == "change_tariff":
            if len(callback_query.data.split(";")) > 1:
                page = callback_query.data.split(";")[-1]
                page = int(page)
            else:
                page = 1
            offset_data = page_size * (page - 1)
            tariffs = db_session.query(Tariff).order_by(Tariff.id.asc()).offset(offset=offset_data).limit(
                limit=page_size).all()
            info_text = (f"Обозначения:\n"
                         f"🟢 - тариф активен\n"
                         f"🟡 - тариф неактивен, но есть пользователь пользующийся им\n"
                         f"🔴 - тариф неактивен\n"
                         f"Список тарифов:\n")
            if len(tariffs) == 0:
                info_text += f"Пусто\n"
            else:
                for tariff in tariffs:
                    if tariff.is_active:
                        info_text += f"🟢 {tariff.id}) {tariff.title}\n"
                    else:
                        is_user = db_session.query(User).filter(User.tariff_id == tariff.id).count()
                        if is_user > 0:
                            info_text += f"🟡 {tariff.id}) {tariff.title}\n"
                        else:
                            info_text += f"🔴 {tariff.id}) {tariff.title}\n"
                reply_markup = keyboard.pagination_keyboard(data_count=len(tariffs),
                                                            data_max=page_size,
                                                            prefix="u_change;change_tariff",
                                                            page=page
                                                            )
                info_text += (
                    "Введите номер тарифа,на который хотите поменять(время дейтсвия тарифа останеться тем же, что была ранее):")
            await state.update_data({"cu_state": "cu_change_tariff"})
        elif ct_state == "off_tariff":
            admin: User = db_session.query(User).filter(User.id == data.get("u_id")).one()
            admin.tariff_status = 2
            db_session.commit_session()
            update_data = datetime.datetime.now()
            admin.updated_at = update_data
            info_text += "Успешно обновлено"
            await callback_query.message.answer(info_text)
            await bot.send_message(chat_id=admin.telegram_id, text=f"Ваша подписка отключена")
            await view(callback_query.message, state, user, admin_id=admin.id)
            return
        elif ct_state == "on_tariff":
            admin: User = db_session.query(User).filter(User.id == data.get("u_id")).one()
            admin.tariff_status = 1
            db_session.commit_session()
            update_data = datetime.datetime.now()
            admin.updated_at = update_data
            info_text += "Успешно обновлено"
            await callback_query.message.answer(info_text)
            await bot.send_message(chat_id=admin.telegram_id, text=f"Ваша подписка включена")
            await view(callback_query.message, state, user, admin_id=admin.id)
            return
        elif ct_state == "delete_tariff":
            admin: User = db_session.query(User).filter(User.id == data.get("u_id")).one()
            admin.tariff_status = 0
            admin.tariff_start = None
            admin.tariff_end = None
            admin.tariff_id = -1
            update_data = datetime.datetime.now()
            admin.updated_at = update_data
            db_session.commit_session()
            info_text += "Успешно обновлено"
            await callback_query.message.answer(info_text)
            await bot.send_message(chat_id=admin.telegram_id, text=f"Ваша подписка удалена")
            await view(callback_query.message, state, user, admin_id=admin.id)
            return
        elif ct_state == "prolong_tariff":
            client: User = db_session.query(User).filter(User.id == data.get("u_id")).one()
            tariff = db_session.query(Tariff).filter(Tariff.id == client.tariff_id).one()
            if client.tariff_end >= datetime.datetime.now():
                client.tariff_end += datetime.timedelta(days=tariff.days)
            else:
                client.tariff_end = datetime.datetime.now() + datetime.timedelta(days=tariff.days)
            update_data = datetime.datetime.now()
            client.updated_at = update_data
            label = f"admin_prolong_tariff:{str(datetime.now().microsecond)}:{tariff}:0"
            transaction = Transaction(created_at=update_data, updated_at=update_data, label=label, user_id=client.id,
                                      is_payment=0, is_admin_check=1, is_close=1)
            db_session.add_model(transaction)
            db_session.commit_session()
            info_text += "Успешно обновлено"
            await callback_query.message.answer(info_text)
            await bot.send_message(chat_id=client.telegram_id, text=f"Ваша подписка продлена")
            await view(callback_query.message, state, user, admin_id=client.id)
            return
        elif ct_state == "deactivate":
            admin: User = db_session.query(User).filter(User.id == data.get("u_id")).one()
            admin.is_admin = False
            info_text += "Успешно обновлено"
            update_data = datetime.datetime.now()
            admin.updated_at = update_data
            db_session.commit_session()
            await callback_query.message.answer(info_text)
            await bot.send_message(chat_id=admin.telegram_id, text=f"Вам отключили админ панель")
            await every(callback_query, state, user)
            return
        elif ct_state == "unblocked":
            admin: User = db_session.query(User).filter(User.id == data.get("u_id")).one()
            admin.is_blocked = False
            info_text += "Успешно обновлено"
            update_data = datetime.datetime.now()
            admin.updated_at = update_data
            db_session.commit_session()
            await callback_query.message.answer(info_text)
            await bot.send_message(chat_id=admin.telegram_id, text=f"Поздровляю вас разблокировали")
            await every(callback_query, state, user)
            return
        elif ct_state == "activate":
            admin: User = db_session.query(User).filter(User.id == data.get("u_id")).one()
            admin.is_admin = True
            admin.is_blocked = False
            info_text += "Успешно обновлено"
            update_data = datetime.datetime.now()
            admin.updated_at = update_data
            db_session.commit_session()
            await callback_query.message.answer(info_text)
            await bot.send_message(chat_id=admin.telegram_id,
                                   text=f"Поздравляю вам доступны админ права(Если вы были заблокированы, то блокировка снята)")
            await every(callback_query, state, user)
            return
        elif ct_state == "blocked":
            info_text += ("Введите причину блокировки:")
            await state.update_data({"cu_state": "cu_blocked"})
        elif ct_state == "list_transaction":
            if len(callback_query.data.split(";")) > 1:
                page = callback_query.data.split(";")[-1]
                page = int(page)
            else:
                page = 1
            offset_data = page_size * (page - 1)
            transactions = db_session.query(Transaction).filter(Transaction.user_id == data.get("u_id")).order_by(
                Transaction.id.desc()).offset(offset=offset_data).limit(
                limit=page_size).all()
            info_text += "Введите номер транзакции для подробной информации о нем\n"

            for transaction in transactions:
                info_text += f"{transaction.id}) {typesTransaction.get(transaction.label.split(':')[0], 'Не известный тип')}\n"
            reply_markup = keyboard.pagination_keyboard(data_count=len(transactions),
                                                        data_max=page_size,
                                                        prefix="u_change;list_transaction",
                                                        page=page)

        await state.update_data({"cu_state": "cu_list_transaction"})
        tmp = ""
        arrayMsg = []
        for value in info_text.split("\n"):
            if len(tmp + value + "\n") <= 4096:
                tmp += value + "\n"
            else:
                arrayMsg.append(tmp)
                tmp = value + "\n"
        arrayMsg.append(tmp)
        for x in range(len(arrayMsg)):
            if x + 1 == len(arrayMsg):
                msg = await callback_query.message.answer(arrayMsg[x], parse_mode=types.ParseMode.HTML,
                                                          reply_markup=reply_markup)
            else:
                await callback_query.message.answer(arrayMsg[x], parse_mode=types.ParseMode.HTML)
        await state.update_data({"last_msg": msg.message_id})
        if flag:
            await AdminStates.change_user.set()

    except Exception as e:
        logger.warning(f"Error change user profile id:{data.get('u_id')} state:{ct_state} - error:{e}")
