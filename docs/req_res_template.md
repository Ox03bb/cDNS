## DNS REQUEST (Query)

```bash
;; HEADER SECTION
ID: 0x1a2b                ; رقم عشوائي يطابق الرد
QR: 0                      ; 0 = طلب (Query)
Opcode: 0                  ; استعلام عادي
AA: 0                      ; غير موثوق (لأنه طلب)
TC: 0
RD: 1                      ; العميل يطلب التكرار (Recursion Desired)
RA: 0
Z: 0
RCODE: 0
QDCOUNT: 1
ANCOUNT: 0
NSCOUNT: 0
ARCOUNT: 0

;; QUESTION SECTION
www.example.com.   IN   A
```
## DNS RESPONSE (Referral with Authority + Additional)

```bash
;; HEADER SECTION
ID: 0x1a2b                ; نفس رقم الطلب
QR: 1                      ; 1 = رد (Response)
Opcode: 0                  ; استعلام عادي
AA: 0                      ; غير موثوق (هذا رد إحالة وليس الخادم النهائي)
TC: 0
RD: 1                      ; تم طلب التكرار
RA: 1                      ; الخادم يسمح بالتكرار
Z: 0
RCODE: 0                   ; لا خطأ
QDCOUNT: 1
ANCOUNT: 0                 ; لا يوجد جواب مباشر
NSCOUNT: 2                 ; سجلين في قسم السلطة (Authority)
ARCOUNT: 2                 ; سجلين في القسم الإضافي (Additional)

;; QUESTION SECTION
www.example.com.   IN   A

;; AUTHORITY SECTION
example.com.     172800   IN   NS   ns1.example.net.
example.com.     172800   IN   NS   ns2.example.net.

;; ADDITIONAL SECTION
ns1.example.net. 172800   IN   A    192.0.2.53
ns2.example.net. 172800   IN   A    192.0.2.54
```