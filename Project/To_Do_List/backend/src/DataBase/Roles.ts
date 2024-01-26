import type { Sequelize } from "sequelize";
import { DataTypes } from "sequelize";

export class Roles {
  public roles: any;
  constructor(sequelize: Sequelize) {
    this.roles = sequelize.define(
      "Roles",
      {
        Role_id: {
          type: DataTypes.INTEGER,
          autoIncrement: true,
          allowNull: false,
          primaryKey: true,
        },
        Name: {
          type: DataTypes.TEXT,
          allowNull: false,
        },
      },
      {
        timestamps: false,
      }
    );
  }
}
