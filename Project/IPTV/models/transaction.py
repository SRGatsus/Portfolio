from sqlalchemy import Column, Integer, BOOLEAN, TEXT, ForeignKey
from models.base import BaseModel



typesTransaction={
    "admin_change_tariff":"Изменение подписики(администратор)",
    "admin_add_tariff":"Добавление подписки(администратор)",
}
class Transaction(BaseModel):
    __tablename__ = "transaction"
    label = Column(TEXT, nullable=False)
    user_id = Column(Integer, ForeignKey('user_account.id', ondelete='SET DEFAULT'), nullable=False, index=True,default=-1)
    is_payment = Column(BOOLEAN, default=False)
    is_admin_check = Column(BOOLEAN, default=False)
    is_close = Column(Integer, default=False)

    def __str__(self):
        from loader import db_session
        from models import Tariff

        text = ""
        label_arr = str(self.label).split(":")
        type = label_arr[0]
        is_tariff = db_session.query(Tariff).filter(Tariff.id == label_arr[-2]).count()
        if is_tariff >0:
            tariff = (db_session.query(Tariff).filter(Tariff.id == label_arr[-2]).one())
            tariff = f"{tariff.id} - {tariff.title}"
        else:
            tariff = "Тариф не найден\n"
        text += (f"ID: {self.id}\n"
                f"Тип: {typesTransaction.get(type,'Не известный тип')}\n"
                 f"Цена: {label_arr[-1]}\n"
                f"Дата создания: {(self.created_at).strftime('%d-%m-%Y в %H:%M')}\n"
                f"Дата обновления: {(self.updated_at).strftime('%d-%m-%Y в %H:%M')}\n"
                f"")
        if type == "admin_change_tariff":
            is_tariff = db_session.query(Tariff).filter(Tariff.id == label_arr[-3]).count()
            if is_tariff > 0:
                tariff_old = (db_session.query(Tariff).filter(Tariff.id == label_arr[-3]).one())
                tariff_old = f"{tariff_old.id} - {tariff_old.title}"
            else:
                tariff_old = "Тариф не найден\n"
            text+=f"Аминистратор заменил подписку с:\n{tariff_old}\nНа:\n{tariff}\n"
        elif type == "admin_add_tariff":
            text += f"Аминистратор добавил подписку:\n{tariff}\n"
        return text

