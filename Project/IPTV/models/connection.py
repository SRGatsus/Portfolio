from sqlalchemy import Column, Integer, BOOLEAN, TEXT, ForeignKey, TIMESTAMP

from models.base import Base


class Connection(Base):
    __tablename__ = "connection"
    id = Column(Integer, nullable=False, unique=True, primary_key=True, autoincrement=True)
    ip_address = Column(TEXT, nullable=False)
    user_id = Column(Integer, ForeignKey('user_account.id', ondelete='SET DEFAULT'), nullable=False, index=True,default=-1)
    connection_time = Column(TIMESTAMP, default=False)
