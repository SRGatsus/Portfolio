import datetime

from loader import db_session
from models import User
from utils.misc.logging import logger
from data.config import MAIN_ADMIN, ADMINS


def count_users() -> int:
    return db_session.query(User).count()


def get_users() -> list[User]:
    query = db_session.query(User).every()
    return list(query)


def get_user(id: int) -> User:
    try:
        return db_session.query(User).filter(User.telegram_id == id).one()
    except Exception as e:
        logger.warning(e)
        return None


def create_user(id: int, username: str = None) -> User:
    is_admin = id == MAIN_ADMIN or id in ADMINS
    create_data = update_data = datetime.datetime.now()
    new_user = User(telegram_id=id, username=username, is_admin=is_admin, created_at=create_data,
                    updated_at=update_data)
    db_session.add_model(new_user)
    db_session.commit_session()
    logger.info(f"New user\n{new_user}")
    return new_user


def update_username(user, username: str = None) -> User:
    user.username = username
    db_session.update(user)
    logger.info(f"Update username @{username} with {user.id} id")
    return user


def get_or_create_user(id: int, username: str = None) -> User:
    user = get_user(id)
    if user:
        return user
    return create_user(id, username)


def checkAdmin(id: int) -> bool:
    try:
        user = db_session.query(User).filter(User.telegram_id == id).one()
    except Exception as e:
        logger.warning(e)
        if id == int(MAIN_ADMIN):
            return True
        return False
    return user.is_admin
