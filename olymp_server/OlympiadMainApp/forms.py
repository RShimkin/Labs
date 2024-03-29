from django import forms
from django.forms.widgets import NumberInput
from datetime import datetime
# from django.core.exceptions import ValidationError

from .models import *
from .langs import prog_langs

class CodeForm(forms.Form):
    # title = forms.CharField(max_length=255, label="Название")
    content = forms.CharField(widget=forms.Textarea(attrs={
        'cols': 140, 'rows': 17, 'class':'form-control', 'id':'ta', 'required':False
    }), label='')
    plang = forms.ChoiceField(choices=prog_langs.choices(), label='') #widget=forms.Select(attrs={'class':'choiceField'}), label='')

class MyCodeForm(forms.Form):
    #title = forms.CharField(max_length=255, label='Название')
    content = forms.CharField(widget=forms.Textarea(attrs={
        'cols': 120, 'rows': 17, 'class':'form-control', 'id':'ta', 'required':False
    }), label='Код')
    plang = forms.ChoiceField(choices=prog_langs.choices(), label='')

'''
class CreateTaskForm(forms.ModelForm):
    class Meta:
        model = Task
        fields = ['name', 'description', 'points', 'until', 'active', 'test']
'''

class CreateTaskForm(forms.Form):
    name = forms.CharField(max_length=255, label='Название задачи')
    short_description = forms.CharField(widget=forms.Textarea(attrs={
        'cols':50, 'rows':2, 'class':'form-control','required':False
    }), label='Краткое описание задачи')
    description = forms.CharField(widget=forms.Textarea(attrs={
        'cols':50, 'rows':10, 'class':'form-control','required':False
    }), label='Описание задачи и требования')
    #points = forms.IntegerField(initial=10, label="Количество очков")
    since = forms.DateTimeField(
        initial = datetime.now(),
        widget = NumberInput(attrs={'type': 'datetime-local'}),
        label = "Активна с")
    until = forms.DateTimeField(
        initial = datetime.now(),
        widget = NumberInput(attrs={'type': 'datetime-local'}),
        label="Активна до")
    tasktype = forms.ChoiceField(
        label = 'Тип задачи',
        required = True,
        choices = TaskType.choices
    )
    #active = forms.BooleanField(initial=True, label="Активна?")

class AddTaskForm(forms.Form):
    name = forms.CharField(max_length=255, label='Название задачи')
    description = forms.CharField(widget=forms.Textarea(attrs={
        'cols':50, 'rows':5, 'class':'form-control','required':False
    }), label='Описание задачи и требования')
    question = forms.CharField(widget=forms.Textarea(attrs={
        'cols':50, 'rows':3, 'class':'form-control','required':False
    }), label='Тестовый вопрос', required=False)
    points = forms.IntegerField(initial=10, label="Количество очков")
    tasktype = forms.ChoiceField(
        label = 'Тип задачи',
        required = True,
        choices = TaskType.choices
    )

class CreateOlympiadForm(forms.Form):
    name = forms.CharField(max_length=255, label="Название олимпиады")
    description = forms.CharField(widget=forms.Textarea(attrs={
        'cols':50, 'rows':7, 'class':'form-control', 'required':False
    }), label="Описание олимпиады")
    since = forms.DateTimeField(
        initial = datetime.now(),
        widget = NumberInput(attrs={'type': 'datetime-local'}),
        label = "Активна с")
    until = forms.DateTimeField(
        initial = datetime.now(),
        widget = NumberInput(attrs={'type': 'datetime-local'}),
        label="Активна до")
    timeout = forms.IntegerField(
        label="Ограничение по времени (мин)",
        required=False,
        widget=forms.NumberInput())
    active = forms.BooleanField(initial=False, label="Активна?", required=False)
    hidden = forms.BooleanField(initial=False, label="Скрыта?", required=False)

class EditStaskForm(forms.ModelForm):
    class Meta:
        model = StandaloneTask
        fields = ('name', 'description', 'since')

class EditOlympiadForm(forms.ModelForm):
    class Meta:
        model = Olympiad
        fields = ('name', 'description', 'since', 'until', 'timeout', 'active')

    since = forms.DateTimeField(
        initial = datetime.now(),
        widget = NumberInput(attrs={'type': 'datetime-local'}),
        label = "Активна с")
    until = forms.DateTimeField(
        initial = datetime.now(),
        widget = NumberInput(attrs={'type': 'datetime-local'}),
        label="Активна до")
    timeout = forms.IntegerField(
        label="Ограничение по времени (мин)",
        required=False,
        widget=forms.NumberInput())
    active = forms.BooleanField(initial=False, label="Активна?", required=False)

class SimpleCodeTestDataForm(forms.Form):
    input = forms.CharField()
    output = forms.CharField()

class CreateTaskForm2(forms.Form):
    name = forms.CharField(max_length=255, label='Название задачи')
    description = forms.CharField(widget=forms.Textarea(attrs={
        'cols':50, 'rows':17, 'class':'form-control','required':False
    }), label='Описание задачи')
    points = forms.IntegerField(initial=10, label="Максимум очков")
    until = forms.DateField(widget=forms.SelectDateWidget(), label="Активна до")
    active = forms.BooleanField(initial=True, label="Активна?")

class ChoiceTestDataForm(forms.Form):
    answer = forms.CharField()

class FileTestForm(forms.Form):
    file = forms.FileField(label="Файл")