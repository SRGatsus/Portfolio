import {genSaltSync, hashSync, compareSync} from "bcryptjs";

export function hashPassword(text: string): string {
  const salt = genSaltSync(12);
  const hashedPassword = hashSync(text, salt);
  return hashedPassword;
}

export function verifyPassword(textAttempted: string, hashedPassword: string): boolean {
  return compareSync(textAttempted, hashedPassword);
}
