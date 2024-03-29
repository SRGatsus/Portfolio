from sqlalchemy.exc import IntegrityError, DataError
from sqlalchemy.orm import Session

from models.base import BaseModel
from utils.misc.logging import logger


class DBSession(object):
    _session: Session

    def __init__(self, session: Session, *args, **kwargs):
        self._session = session

    def query(self, *entities, **kwargs):
        return self._session.query(*entities, **kwargs)

    def add_model(self, model: BaseModel, need_flush: bool = False):
        self._session.add(model)

        if need_flush:
            self._session.flush([model])

    def delete_model(self, model: BaseModel):
        if model is None:
            logger.warning(f'{__name__}: model is None')

        try:
            self._session.delete(model)
        except IntegrityError as e:
            logger.error(f'`{__name__}` {e}')
        except DataError as e:
            logger.error(f'`{__name__}` {e}')

    def commit_session(self, need_close: bool = False):
        try:
            self._session.commit()
        except IntegrityError as e:
            logger.error(f'`{__name__}` {e}')
            raise
        except DataError as e:
            logger.error(f'`{__name__}` {e}')
            raise

        if need_close:
            self.close_session()

    def close_session(self):
        try:
            self._session.close()
        except IntegrityError as e:
            logger.error(f'`{__name__}` {e}')
            raise
        except DataError as e:
            logger.error(f'`{__name__}` {e}')
            raise
