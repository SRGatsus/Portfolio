FROM node

WORKDIR /app

COPY package.json /app

RUN npm instal

COPY . .



EXPOSE 8080

CMD ["npm","start"]