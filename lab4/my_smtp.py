import smtplib
import sys
from email.mime.application import MIMEApplication
from email.mime.multipart import MIMEMultipart
from email.mime.base import MIMEBase
from email.utils import COMMASPACE, formatdate
from email import encoders


if __name__ == "__main__":
    mail_user = sys.argv[1] #123@mail.com
    mail_password = sys.argv[2] #asdf
    send_to = [sys.argv[3]] #mail.to.send@mail.com ...
    filename=sys.argv[4] #send.txt

    part = MIMEBase('application', "octet-stream")
    send_from = mail_user  

    part.set_payload(open(filename, "rb").read())
    encoders.encode_base64(part)
    part.add_header('Content-Disposition', 'attachment; filename="{}"'.format(filename))

    letter = MIMEMultipart()
    letter['From'] = send_from
    letter['To'] = COMMASPACE.join(send_to)
    letter['Date'] = formatdate(localtime=True)
    letter['Subject'] = 'Важное письмо!'
    letter.attach(part)
 
    try:
        server_ssl = smtplib.SMTP_SSL('smtp.gmail.com', 465)
        server_ssl.ehlo()
        server_ssl.login(mail_user, mail_password)
        server_ssl.sendmail(send_from, send_to, letter.as_string())
        server_ssl.close()

        print('Отправлено')
    except Exception:
        print("Ошибка!")
