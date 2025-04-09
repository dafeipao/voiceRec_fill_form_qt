from pypinyin import lazy_pinyin

def convert_to_pinyin(text):
    return ' '.join(lazy_pinyin(text))
