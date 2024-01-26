from sqlalchemy import Column, Integer, BOOLEAN, TEXT, ForeignKey, TIMESTAMP

from models.base import BaseModel


class Settings(BaseModel):
    __tablename__ = "setting"
    max_connection = Column(Integer, nullable=False)
