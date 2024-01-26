from sqlalchemy import Column, Integer, TIMESTAMP, BOOLEAN, ForeignKey, TEXT
from datetime import datetime
from models.base import BaseModel


class User(BaseModel):
    __tablename__ = "user_account"
    telegram_id = Column(Integer, nullable=False, unique=True)
    username = Column(TEXT, nullable=False)
    tariff_id = Column(Integer, ForeignKey('tariff.id', ondelete='SET DEFAULT'), nullable=False, index=True, default=-1)
    tariff_start = Column(TIMESTAMP, nullable=True)
    tariff_end = Column(TIMESTAMP, nullable=True)
    tariff_status = Column(Integer, default=0)
    is_admin = Column(BOOLEAN, default=0)
    is_blocked = Column(BOOLEAN, default=0)

    def __str__(self):
        text = (f"ID: {self.id}\n"
                f"Telegram ID: {self.telegram_id}\n"
                f"Ник: {self.username}\n"
                f"Дата регистрации: {(self.created_at).strftime('%d-%m-%Y в %H:%M')}\n")
        return text
