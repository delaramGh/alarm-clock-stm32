from kivy.app import App
from kivy.uix.label import Label
from kivy.uix.floatlayout import FloatLayout
from kivy.uix.scatter import Scatter
from kivy.uix.textinput import TextInput
from kivy.uix.boxlayout import BoxLayout
import random


class myWidget(BoxLayout):
    def change_label_color(self, *args):
        color = [random.random() for i in range(3)] + [1]
        label = self.ids["my_label"]
        label.color = color





class TutorialApp(App):
    def build(self):
        # b = BoxLayout(orientation='vertical')
        # s = Scatter()
        # l = Label(text='default', font_size=150, color=(0.9, 0.5, 0.5, 1))
        # t = TextInput(size_hint_y=None ,height=100)
        # f = FloatLayout()

        # b.add_widget(t)
        # f.add_widget(s)
        # s.add_widget(l)
        # b.add_widget(f)

        # t.bind(text=l.setter('text'))
        return myWidget()


if __name__ == "__main__":
    TutorialApp().run()
