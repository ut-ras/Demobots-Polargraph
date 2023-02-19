# CRUD driver for CloudSQL with SQLAlchemy. Each db.Model class corresponds to a table in the database. 
# CRUD stands for Create, Read, Update, Delete


from flask import Flask, session
from flask_sqlalchemy import SQLAlchemy

builtin_list = list
db = SQLAlchemy()


def init_app(app):
    # Disable track modifications, as it unnecessarily uses memory.
    app.config.setdefault('SQLALCHEMY_TRACK_MODIFICATIONS', False)
    db.init_app(app)
    _create_tables(app)


def from_sql(row):
    """Translates a SQLAlchemy model instance into a dictionary"""
    data = row.__dict__.copy()
    data.pop('_sa_instance_state')
    return data

def _create_tables(app):
    with app.app_context():
        db.create_all()
        print("Admins: " + str(list_admins()))
        #print("Drawings: " + str(list_drawings()))
    print("All tables created")



# Drawing

class Drawing(db.Model):
    __tablename__ = 'drawings'
    id = db.Column(db.Integer, primary_key=True)
    title = db.Column(db.String(255))
    numPoints = db.Column(db.String(255))
    coordinatesX = db.Column(db.Text)
    coordinatesY = db.Column(db.Text)

    def __repr__(self):
        return "<Drawing(title='%s', numPoints=%s)" % (self.title, self.numPoints)


def list_drawings(limit=10, cursor=None):
    print ("Listing drawings from SQL")
    cursor = int(cursor) if cursor else 0
    query = (Drawing.query.order_by(Drawing.title).limit(limit).offset(cursor))
    drawings = builtin_list(map(from_sql, query.all()))
    next_page = cursor + limit if len(drawings) == limit else None
    #print ("Listing drawings: " + drawings + " next page: " + next_page)
    return (drawings, next_page)


def read_drawing(id):
    result = Drawing.query.get(id)
    if not result:
        return None
    return from_sql(result)


def create_drawing(data):
    drawing = Drawing(**data)
    db.session.add(drawing)
    db.session.commit()
    return from_sql(drawing)


def update_drawing(data, id):
    drawing = Drawing.query.get(id)
    for k, v in data.items():
        setattr(drawing, k, v)
    db.session.commit()
    return from_sql(drawing)


def delete_drawing(id):
    Drawing.query.filter_by(id=id).delete()
    db.session.commit()



# Admin

class Admin(db.Model):
    __tablename__ = 'adminlist'
    email = db.Column(db.String(255), primary_key=True)

    def __repr__(self):
        return "<Admin(email='%s')" % (self.email)

def is_user_admin():
    if 'profile' in session:
        profile = session['profile']
        email = profile['emails'][0]['value']
        admins = list_admins()
        for admin in admins:
            if email == admin['email']:
                return True
    return False


def list_admins():
    print ("Listing admins from SQL")
    query = (Admin.query.order_by(Admin.email))
    admins = builtin_list(map(from_sql, query.all()))
    return admins


def read_admin(email):
    result = Admin.query.get(email)
    if not result:
        return None
    return from_sql(result)

def create_admin(data):
    admin = Admin(**data)
    db.session.add(admin)
    db.session.commit()
    return from_sql(admin)


def update_admin(data, email):
    admin = Admin.query.get(email)
    for k, v in data.items():
        setattr(admin, k, v)
    db.session.commit()
    return from_sql(admin)


def delete_admin(email):
    Admin.query.filter_by(email=email).delete()
    db.session.commit()
