import type {Sequelize} from "sequelize";
import {DataTypes} from "sequelize";
import {hashPassword} from "../Util";

export class Users {
  public users: any;

  constructor(sequelize: Sequelize) {
    this.users = sequelize.define(
      "Users",
      {
        User_id: {
          type: DataTypes.INTEGER,
          autoIncrement: true,
          allowNull: false,
          primaryKey: true,
        },
        Email: {
          type: DataTypes.TEXT,
          allowNull: false,
        },
        Password: {
          type: DataTypes.TEXT,
          allowNull: true,
        },
        Role_id: {
          type: DataTypes.INTEGER,
          allowNull: false,
        },
        Name: {
          type: DataTypes.TEXT,
          allowNull: true,
        },
        LastName: {
          type: DataTypes.TEXT,
          allowNull: true,
        },
        SecretKey: {
          type: DataTypes.TEXT,
          allowNull: true,
        },
      },
      {
        timestamps: false,
      }
    );
  }

  public getAllUser() {
    return this.users.findAll();
  }

  public getUserByEmail(email: string) {
    return this.users.findOne({
      where: {
        Email: email,
      },
    });
  }

  public getUserById(id: string) {
    return this.users.findOne({
      where: {
        User_id: id,
      },
    });
  }

  public addUser(userData: any) {
    return this.users.create({
      Email: userData.email,
      Name: "User Name",
      LastName: "User LastName",
      Password: hashPassword(userData.password),
      Role_id: 0,
      SecretKey: hashPassword(userData.secretKey),
    });
  }
}
