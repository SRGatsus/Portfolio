import type { Sequelize } from "sequelize";
import { DataTypes } from "sequelize";
import {hashPassword} from "../Util";

export class Tasks {
  public tasks: any;
  constructor(sequelize: Sequelize) {
    this.tasks = sequelize.define(
      "Tasks",
      {
        Task_id: {
          type: DataTypes.INTEGER,
          autoIncrement: true,
          allowNull: false,
          primaryKey: true,
        },
        Text: {
          type: DataTypes.TEXT,
          allowNull: false,
        },
        User_id: {
          type: DataTypes.INTEGER,
          allowNull: false,
        },
        State: {
          type: DataTypes.TEXT,
          allowNull: false,
        },
      },
      {
        timestamps: false,
      }
    );
  }
  public addTask(taskData: any) {
    return this.tasks.create({
      Text: taskData.text,
      User_id: taskData.user_id,
      State:taskData.state,
    });
  }
  public getAllTask(user_id:string) {
    return this.tasks.findAll({
      where: {
        User_id: user_id,
      },
    });
  }
}
