import ip_lib

ip_lib.load_dict("dict/col_iplib_global.txt")

print ip_lib.get_country("180.214.232.50")
raw_input("xxx")
print ip_lib.cleanup()
raw_input("xxx")
print ip_lib.get_country("180.214.232.50")
ip_lib.load_dict("dict/GIP-lib-latest.txt")
print ip_lib.get_country("180.214.232.50")

ip = ip_lib.IP2Location("dict/GIP-lib-latest.txt")
print ip.get_country("180.214.232.50")
raw_input("xxx")
print ip.cleanup()
raw_input("xxx")
print ip.get_country("180.214.232.50")

del ip
raw_input("xxx")

#import re
#pattern = re.compile(r'\||:')
#def callback(line):
#    fields = pattern.split(line)
#    return fields[0], fields[1], fields[2]
#
#ip = ip_lib.IP2Location("dict/col_iplib_global.txt", callback)
#print ip.get_country("180.214.232.50")
#raw_input("xxx")
#print ip.cleanup()
#raw_input("xxx")
#print ip.get_country("180.214.232.50")
