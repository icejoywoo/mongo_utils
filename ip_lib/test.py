import ip_lib
ip_lib.load_dict("dict/GIP-lib-latest.txt")

print ip_lib.get_country("180.214.232.50")
raw_input("xxx")
print ip_lib.cleanup()
raw_input("xxx")
print ip_lib.get_country("180.214.232.50")
