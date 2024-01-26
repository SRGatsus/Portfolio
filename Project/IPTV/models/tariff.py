from ipytv import playlist
from sqlalchemy import Column, Integer, BOOLEAN, FLOAT, TEXT

from models.base import BaseModel


class Tariff(BaseModel):
    __tablename__ = "tariff"
    price = Column(FLOAT, nullable=False)
    title = Column(TEXT, default="")
    description = Column(TEXT, default="")
    days = Column(Integer, default=30)
    is_active = Column(BOOLEAN, default=1)
    path_file = Column(TEXT, default="")
    list_channel = Column(TEXT, default="")

    def __str__(self):
        #pl = playlist.loads(self.list_channel)
        count = len(str(self.list_channel).split("#EXTINF:"))
        #unique_numbers = list(set([channel.attributes.get('group-title') for channel in pl.get_channels()]))
        text = (f"ID: {self.id or ''}\n"
                f"{self.title}\n"
                f"Срок действия: {self.days}\n"
                f"Цена: {self.price} руб.\n"
                f"Кол-во каналов: {count-1}\n"
                f"{self.description}\n"
                )
        return text
