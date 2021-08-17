import dayjs from 'dayjs';

function Task(id, description, isImportant = false, isPrivate = true, isCompleted = false, deadline = "", user) {
  this.id = id;
  this.description = description;
  this.isImportant = isImportant || false;
  this.isPrivate = isPrivate || false;
  this.isCompleted = isCompleted || false;
  this.deadline = deadline && dayjs(deadline);
  this.user = user;

  this.toString = () => {
    return `ID: ${this.id}, Description: ${this.description}, Important: ${this.isImportant}, Private: ${this.isPrivate}, Completed: ${this.isCompleted}, Deadline: ${this.deadline}, User: ${this.user}`;
  }
}

export default Task;
