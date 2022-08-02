import stanza
import sys
import os


def exception_decorator(function):
    def wrapper(*args, **kwargs):
        try:
            return function(*args, **kwargs)
        except:
            print("Exception:", sys.exc_info()[0])
            raise

    return wrapper


class Stanza:

    def __init__(self):
        root = os.path.abspath(os.path.dirname(os.path.abspath(__file__)))
        directory = os.path.join(root, "models")

        config = {
            "processors": "tokenize,pos,lemma,depparse,ner",
            "lang": "en",
            "models_dir": directory,
            "dir": directory,
            "use_gpu": False
        }

        self.nlp = stanza.Pipeline(**config)

    @exception_decorator
    def run(self, text):
        """
            Функция принимает текст на английском языке из N >= 1 предложений.
            :return: двумерный список слов, каждое из которых представлено полями:
                -id - порядковый номер в предложении
                -lemma - лемма (в н.ф.)
                -upos - universal part-of-sentence
                -feats - morphological features
                -head - номер head слова в предложении
                -deprel - dependency relations
            """

        document = self.nlp(text)
        result = [
            [
                {
                    "id": int(word.id),
                    "lemma": word.lemma,
                    "word": word.text,
                    "upos": word.upos,
                    "feats": word.feats,
                    "head": int(word.head),
                    "deprel": word.deprel,
                } for word in sentence.words
            ] for sentence in document.sentences
        ]

        return result