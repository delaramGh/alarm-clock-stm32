from kivy.app import App
from kivy.uix.label import Label
from kivy.uix.floatlayout import FloatLayout
from kivy.uix.scatter import Scatter


class TutorialApp(App):
    def build(self):
        f = FloatLayout()
        s = Scatter()
        l = Label(text='Hello Android!', font_size=100, color=(0.9, 0.5, 0.5, 1))

        f.add_widget(s)
        s.add_widget(l)
        return f


if __name__ == "__main__":
    TutorialApp().run()
