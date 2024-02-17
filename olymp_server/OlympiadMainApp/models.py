from django.db import models
from djongo import models as dmodels
from django.urls import reverse
from django.contrib.auth.models import User
from django.utils.translation import gettext_lazy as _
#import bson

#from OlympiadUsers.models import Profile
from Config import config


LANG_CHOICES = (
    ('C++', 'C++'),
    ('PYTHON', 'Python')
)

class TaskType(models.TextChoices):
    SIMPLECHOICE = 'SCH', _('Выбор ответа')
    MULTIPLECHOICE = 'MCH', _('Множественный выбор')
    FULLANSWER = 'FUL', _('Развёрнутый ответ')
    CODESIMPLETEST = 'CST', _('Тестирование сравнением')
    CODEDOUBLETEST = 'CDT', _('Тестирование проверкой')

def getDefaultTestJson():
    return {}

class TestData(dmodels.Model):
    type = dmodels.CharField(max_length=3, choices=TaskType.choices, default=TaskType.CODESIMPLETEST, verbose_name="Тип задачи")
    choices = dmodels.JSONField(null=True, blank=True, default=getDefaultTestJson())
    testinput = dmodels.CharField(max_length=1000, primary_key=True, default=None, verbose_name="Текстовый ввод")
    testoutput = dmodels.CharField(max_length=1000, verbose_name="Текстовый вывод")

    objects = dmodels.DjongoManager()

    def __str__(self):
        return f"{self.testinput} -> {self.testoutput}"

    def __getitem__(self, name):
       return getattr(self, name)
    
    class Meta:
        managed = False

'''
class TaskResult(dmodels.Model):
    #_id = dmodels.ObjectIdField(primary_key=True, default=bson.ObjectId(b'foo-bar-quux'))
    uname = dmodels.CharField(max_length=255, verbose_name="Имя пользователя")
    taskname = dmodels.CharField(max_length=255, verbose_name="Название задачи")
    points = dmodels.PositiveSmallIntegerField(verbose_name="Очки пользователя", default=0)
    time = dmodels.DateTimeField(auto_now_add=True)

    objects = dmodels.DjongoManager()

    def __str__(self):
        return f"{self.uname} scored {self.points} in task {self.taskname}"
    
    class Meta:
        indexes = [
            models.Index(fields=['uname'], name='res_name_idx'),
            models.Index(fields=['taskname'], name='res_task_idx')
        ]
'''

class StandaloneTaskResult(dmodels.Model):
    _id = dmodels.ObjectIdField(primary_key=True)
    user = dmodels.ForeignKey(User, on_delete=dmodels.DO_NOTHING, default=None, null=True, verbose_name="Чей результат")
    username = dmodels.CharField(max_length=255, verbose_name="Чей результат (имя пользователя)")
    taskname = dmodels.CharField(max_length=255, verbose_name="Для какой задачи (название)")
    points = dmodels.PositiveIntegerField(verbose_name='Вес в очках')
    started = dmodels.DateTimeField(auto_now_add=False, default=None, verbose_name='Время начала выполнения')
    finished = dmodels.DateTimeField(auto_now_add=False, default=None, verbose_name='Время начала выполнения')
    attempts = dmodels.PositiveIntegerField(verbose_name='количество попыток')
    inner_attempts = dmodels.PositiveIntegerField(verbose_name='количество внутренних отправлений')
    success = dmodels.BooleanField(default=False, verbose_name='статус выполнения')

    objects = dmodels.DjongoManager()

    class Meta:
        verbose_name = 'Пользовательский результат по задаче'
        verbose_name_plural = 'результаты'
        ordering = ['-points']
        indexes = [
            #models.Index(fields=['guid']),
            #models.Index(fields=['slug'], name='task_slug_idx'),
        ]

    def get_username(self):
        if self.user:
            return self.username
        return "[Аноним]"

class StandaloneTask(dmodels.Model):
    _id = dmodels.ObjectIdField(primary_key=True)
    name = dmodels.CharField(max_length=255, unique=True, verbose_name="Название задачи")
    short_description = dmodels.TextField(max_length=500, verbose_name="Краткое описание задачи")
    description = dmodels.TextField(max_length=10000, verbose_name='Полное описание задачи')
    question = dmodels.TextField(max_length=1000, default=None, verbose_name="Вопрос задачи")
    points = dmodels.PositiveIntegerField(default=config.STANDALONE_POINTS, verbose_name='Вес в очках')
    active = dmodels.BooleanField(default=True)
    creator = dmodels.ForeignKey(User, on_delete=dmodels.DO_NOTHING, default=None, verbose_name="Создатель задачи") #null = True
    tests = dmodels.ArrayField(model_container=TestData, default=None, verbose_name="Тесты")
    created = dmodels.DateTimeField(auto_now_add=True, verbose_name="Время создания")
    since = dmodels.DateTimeField(default=None, verbose_name="Активна с")
    until = dmodels.DateTimeField(default=None, verbose_name="Активна до")
    results = dmodels.ArrayReferenceField(to=StandaloneTaskResult, default=None, on_delete=models.DO_NOTHING, verbose_name="Результаты пользователей")
    results_number = dmodels.PositiveBigIntegerField(default=0, verbose_name="Количество успешных результатов")
    hidden = dmodels.BooleanField(default=False, verbose_name="Закрытый доступ")
    tasktype = dmodels.CharField(max_length=3, choices=TaskType.choices, default=TaskType.CODESIMPLETEST)

    objects = dmodels.DjongoManager()

    def get_view_url(self):
        return reverse('viewstask', kwargs={'task_oid': str(self._id)})
    
    def get_edit_url(self):
        return reverse('editstask', kwargs={'task_oid': str(self._id)})

    def __str__(self):
        return f"stask #{str(self._id)}"

    class Meta:
        verbose_name = 'Задача'
        verbose_name_plural = 'Задачи'
        ordering = ['created']

class InnerTaskResult(dmodels.Model):
    _id = dmodels.ObjectIdField(primary_key=True)
    user = dmodels.ForeignKey(User, on_delete=dmodels.DO_NOTHING, default=None, null=True, verbose_name="Чей результат")
    task = dmodels.GenericObjectIdField(primary_key=False, default=None, verbose_name="Ссылка на задачу")
    points = dmodels.PositiveIntegerField(default=0, verbose_name='Набранные очки')
    finished = dmodels.DateTimeField(auto_now_add=False, default=None, verbose_name='Время успешного выполнения')
    inner_attempts = dmodels.PositiveIntegerField(verbose_name='количество попыток')
    success = dmodels.BooleanField(default=False, verbose_name='статус выполнения')

    objects = dmodels.DjongoManager()

    class Meta:
        verbose_name = 'Пользовательский результат по задаче'
        verbose_name_plural = 'результаты'
        #ordering = ['-points']
        indexes = [
            #models.Index(fields=['guid']),
            #models.Index(fields=['slug'], name='task_slug_idx'),
        ]

    '''def get_username(self):
        if self.user:
            return self.username
        return "[Аноним]"'''

class InnerTask(dmodels.Model):
    _id = dmodels.ObjectIdField(primary_key=True)
    name = dmodels.CharField(max_length=255, unique=False, verbose_name="Название задачи")
    short_description = dmodels.TextField(max_length=500, verbose_name="Краткое описание задачи")
    description = dmodels.TextField(max_length=10000, verbose_name='Полное описание задачи')
    question = dmodels.TextField(max_length=1000, default=None, verbose_name="Вопрос задачи")
    points = dmodels.PositiveIntegerField(default=config.STANDALONE_POINTS, verbose_name='Очки (вес)')
    tests = dmodels.ArrayField(model_container=TestData, default=None, verbose_name="Тесты")
    results = dmodels.ArrayReferenceField(to=InnerTaskResult, default=None, on_delete=models.DO_NOTHING, verbose_name="Результаты пользователей")
    olympiad_id = dmodels.GenericObjectIdField(default=None, primary_key=False, verbose_name="Ссылка на олимпиаду")
    tasktype = dmodels.CharField(max_length=3, choices=TaskType.choices, default=TaskType.CODESIMPLETEST)
    timeout = dmodels.DurationField()
    again = dmodels.BooleanField(default=False)

    objects = dmodels.DjongoManager()

    def __str__(self):
        return f"itask #{str(self._id)}"

    class Meta:
        verbose_name = 'Задача'
        verbose_name_plural = 'Задачи'
        #ordering = ['created']
        #indexes = [
            #models.Index(fields=['slug'], name='task_slug_idx'),
        #]

class OlympiadResult(dmodels.Model):
    _id = dmodels.ObjectIdField(primary_key=True)
    user = dmodels.ForeignKey(User, on_delete=dmodels.DO_NOTHING, default=None, null=True, verbose_name="Чей результат")
    olympiad = dmodels.GenericObjectIdField(primary_key=False, default=None, verbose_name="Ссылка на олимпиаду")
    points = dmodels.PositiveIntegerField(default=0, verbose_name='Набранные очки')
    started = dmodels.DateTimeField(auto_now_add=False, default=None, verbose_name='Время начала выполнения')
    finished = dmodels.DateTimeField(auto_now_add=False, default=None, verbose_name='Время успешного выполнения')
    attempts = dmodels.PositiveIntegerField(verbose_name='количество попыток')
    task_results = dmodels.ArrayReferenceField(to=InnerTaskResult, default=None, verbose_name='Результаты по задачам')

    objects = dmodels.DjongoManager()

    def __str__(self):
        return f"OR #{str(self._id)}"

    class Meta:
        verbose_name = 'Пользовательский результат по задаче'
        verbose_name_plural = 'результаты'
        ordering = ['-points']
        indexes = [
            #models.Index(fields=['guid']),
            #models.Index(fields=['slug'], name='task_slug_idx'),
        ]


class Olympiad(dmodels.Model):
    _id = dmodels.ObjectIdField(primary_key=True)

    name = dmodels.CharField(max_length=255, unique=True, verbose_name="Название олимпиады")
    short_description = dmodels.TextField(max_length=500, verbose_name="Краткое описание олимпиады")
    description = dmodels.TextField(max_length=10000, verbose_name='Полное описание олимпиады')

    created = dmodels.DateTimeField(auto_now_add=True, verbose_name="Время создания")
    creator = dmodels.ForeignKey(User, on_delete=dmodels.SET_NULL, default=None, null=True, verbose_name="Создатель")

    hidden = dmodels.BooleanField(default=False, verbose_name="Закрытый доступ")
    since = dmodels.DateTimeField(default=None, verbose_name="Активна с")
    until = dmodels.DateTimeField(default=None, verbose_name="Активна до")
    active = dmodels.BooleanField(default=True)
    finished = dmodels.BooleanField(default=False)
    access = dmodels.JSONField(default=getDefaultTestJson(), blank=True)
    timeout = dmodels.DurationField(default=None)

    results = dmodels.ArrayReferenceField(to=OlympiadResult, default=None, on_delete=models.DO_NOTHING, 
        verbose_name="Результаты участников", related_name="olympres")
    results_number = dmodels.PositiveBigIntegerField(default=0, verbose_name="Количество успешных результатов")
    
    tasks = dmodels.ArrayReferenceField(to=InnerTask, default=None, on_delete=models.DO_NOTHING, verbose_name="Список задач")

    objects = dmodels.DjongoManager()

    def get_absolute_url(self):
        return reverse('olympiad', kwargs={'olymp_oid': str(self._id)})
    
    def get_view_url(self):
        return reverse('viewolympiad', kwargs={'olymp_oid': str(self._id)})
    
    def get_edit_url(self):
        return reverse('editolympiad', kwargs={'olymp_oid': str(self._id)})

    def __str__(self):
        return f"olympiad {self.name}"

    class Meta:
        verbose_name = 'Олимпиада'
        verbose_name_plural = 'Олимпиады'
        ordering = ['created']
        indexes = [
            models.Index(fields=['name']),
        ]

class OlympiadViewModel():
    def __init__(self, olymp, user):
        self.view_url = olymp.get_view_url()
        self.edit_url = olymp.get_edit_url()
        self.name = olymp.name
        self.count = olymp.results_number #len(olymp.results.all())
        self.creator = olymp.creator.username
        self.created = olymp.created
        self.since = olymp.since
        self.until = olymp.until
        self.own = user is not None and olymp.creator == user

class TaskViewModel():
    def __init__(self, task):
        self.url = task.get_absolute_url()
        self.name = task.name
        self.count = len(task.results.all())
        self.creator = task.creator.username if task.creator != None else ''
        self.created = task.created

class StandaloneTaskViewModel():
    def __init__(self, task):
        self.view_url = task.get_view_url()
        self.edit_url = task.get_edit_url()
        self.name = task.name
        self.count = task.results_number
        self.creator = task.creator.username if task.creator != None else ''
        self.created = task.created
        

class StandaloneTaskResultViewModel():
    def __init__(self, taskresult):
        self.username = taskresult.get_username()
        self.points = taskresult.points
        self.time = taskresult.finished - taskresult.started