import datetime

from loader import db_session,engine
from models.base import Base
from models import Settings

if __name__ == "__main__":
    Base.metadata.create_all(engine)
    is_settings = db_session.query(Settings).count()
    if is_settings == 0:
        create_data = update_data = datetime.datetime.now()
        settings = Settings(max_connection=2,created_at=create_data,updated_at = update_data)
        db_session.add_model(settings)
        db_session.commit_session()

